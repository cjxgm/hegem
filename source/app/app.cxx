#include "../lib/imgui.hxx"
#include "../scene/parser.hxx"
#include "../scene/view.hxx"
#include "../image/image.hxx"
#include "../tool/journal.hxx"
#include "../tool/tile.hxx"
#include "../tool/scheduler.hxx"
#include "../tool/desktop.hxx"
#include "../glu/states.hxx"
#include "../raytracer/raytracer.hxx"
#include "../raytracer/shade.hxx"
#include "../pathtracer/pathtracer.hxx"
#include "../rasterizer/rasterizer.hxx"
#include "../rasterizer/state.hxx"
#include "../swrast/rasterizer.hxx"
#include "../skein/editor.hxx"
#include "../silo/editor.hxx"
#include "app.hxx"
#include "glfw.hxx"
#include "hdr-texture.hxx"
#include "visualization.hxx"
#include "statistics.hxx"
#include "../tool/unreachable.macro.hxx"
#include <map>
#include <list>
#include <deque>
#include <array>
#include <mutex>
#include <utility>      // for std::forward and std::move
#include <functional>
#include <algorithm>
#include <memory>
#include <thread>

namespace hegem::app
{
    inline namespace
    {
        using tool::journal;
        using image::image_rgb;
        using image::color::linear_rgb;
        using scene::scene_type;
        using scene::view_type;

        journal j() { return {"APP"}; }

        // Process at most `frame_task_capacity` number of tasks per frame
        static constexpr auto framerate_history_size = 60*2;
        float const background[] = { 0.2667, 0.5333, 1.0000, 0.0000 };

        using gl_job = tool::possibly_canceled_job;

        struct context
        {
            std::deque<hdr_texture> images;
            std::deque<tool::task_io> images_io;
            std::list<visualization> visualizations;
            tool::receiver<gl_job> rx_gl;
            tool::task_manager<tool::pool_scheduler> tman{tool::pool_scheduler{(int) std::max(4u, std::thread::hardware_concurrency())}};
            int tile_size[2] = {64, 64};
            int morphing_tile_size[2] = {128, 128};
            int batch_samples = 16;
            int frame_task_capacity = 64;
            std::array<float, framerate_history_size> framerate_history{};
            int framerate_history_offset{};
            tool::desktop_subsystem desktop{};
            skein::editor skein_editor;
            silo::editor silo_editor{&desktop, morphing_tile_size};
            visualization* skein_visualization{};
            tool::receiver<tool::chosen_file_message> skein_file_dialog_rx;
            bool skein_showing_open_file_dialog{};
            bool skein_showing_save_file_dialog{};

            static constexpr auto skein_file_dialog_open_serial = (tool::u64) 0;
            static constexpr auto skein_file_dialog_save_serial = (tool::u64) 1;

            static context& instance()
            {
                static context ctx;
                return ctx;
            }

            ~context()
            {
                j() << "context: (dtor)\n";
                j() << "canceling raytracing and swrast process...\n";
                for (auto& io: images_io) {
                    io.cancel();
                }
                images_io.clear();
                for (auto& vi: visualizations) {
                    vi.reset_raytracing_task_io();
                    vi.reset_swrast_task_io();
                }

                j() << "closing desktop subsystem...\n";
                tool::defer_close_desktop_subsystem(&desktop);
                while (tool::poll_desktop_subsystem(&desktop)) {}
            }

        private:
            context()
            {
                j() << "context: (ctor)\n";
                tool::open_desktop_subsystem(&desktop);
            }
        };

        namespace job
        {
            struct upload
            {
                hdr_texture& hdr;
                image_rgb image;
                tool::tile tile;

                auto operator () () const -> void
                {
                    gl::texture_sub_image2d(
                            hdr.tex,
                            0,
                            tile.x, tile.y, tile.w, tile.h,
                            gl::rgb, gl::float_,
                            image.data());
                    hdr.unmark(tile);
                }
            };

            struct mark_as_working_tile
            {
                hdr_texture& hdr;
                tool::tile tile;

                auto operator () () const -> void
                {
                    hdr.mark(tile);
                }
            };
        }

        auto render_view(scene_type const& scene, view_type view) -> void
        {
            using task_type = tool::task_type<gl_job>;
            auto& ctx = context::instance();
            auto& images = ctx.images;
            auto& images_io = ctx.images_io;
            auto& tman = ctx.tman;
            auto name = "[" + std::to_string(view.bounces) + "] " + scene.name + ": " + view.name;

            images.emplace_back(view.size.x, view.size.y);
            auto& hdr_depth = images.back();
            hdr_depth.blackpoint = glm::vec3{10};
            hdr_depth.whitepoint = glm::vec3{-10};
            hdr_depth.name = name + " (depth)";

            images.emplace_back(view.size.x, view.size.y);
            auto& hdr_normal = images.back();
            hdr_normal.whitepoint = glm::vec3{2};
            hdr_normal.name = name + " (normal)";

            images.emplace_back(view.size.x, view.size.y);
            auto& hdr_combined = images.back();
            hdr_combined.name = std::move(name);

            auto make_task = [&] (auto view, auto tile) {
                return [&, view, tile] (auto tx, auto shared_canceled) {
                    auto send_job = [&] (auto&& job) {
                        tx.send(gl_job{
                            shared_canceled,
                            std::move(job),
                        });
                    };

                    send_job(job::mark_as_working_tile{ hdr_combined, tile });
                    send_job(job::mark_as_working_tile{ hdr_depth, tile });
                    send_job(job::mark_as_working_tile{ hdr_normal, tile });

                    auto [image, buf] = raytracer::raytrace(shared_canceled, scene, view, tile);
                    if (shared_canceled->load()) return;
                    send_job(job::upload{ hdr_combined, std::move(image), tile });

                    auto depth = raytracer::shade_depth(buf, view);
                    if (shared_canceled->load()) return;
                    send_job(job::upload{ hdr_depth, std::move(depth), tile });

                    auto normal = raytracer::shade_normal(buf, view);
                    if (shared_canceled->load()) return;
                    send_job(job::upload{ hdr_normal, std::move(normal), tile });
                };
            };

            auto [tile_w, tile_h] = ctx.tile_size;
            std::vector<task_type> tasks;
            for (auto& tile: tool::tile_iterator{tile_w, tile_h, view.size.x, view.size.y}) {
                tasks.emplace_back(make_task(view, tile));
            }

            images_io.emplace_back(tman.group(ctx.rx_gl.tx(), std::move(tasks)));
        }

        tool::task_io raytrace_combined_view_progressively(scene_type scene, view_type view, hdr_texture& hdr)
        {
            using task_type = tool::task_type<gl_job>;
            auto& ctx = context::instance();
            auto& tman = ctx.tman;
            auto shared_scene = std::make_shared<scene_type>(std::move(scene));
            shared_scene->rebuild_cache();

            auto make_task = [&hdr, &shared_scene] (bool should_mark, int samples, auto view, auto tile) {
                view.samples = samples;
                return [&hdr, view, tile, should_mark, shared_scene] (auto tx, auto shared_canceled) {
                    if (should_mark) {
                        tx.send(gl_job{
                            shared_canceled,
                            job::mark_as_working_tile{ hdr, tile },
                        });
                    }

                    auto [image, _] = raytracer::raytrace(shared_canceled, *shared_scene, view, tile);
                    (void) _;

                    tx.send(gl_job{
                        shared_canceled,
                        job::upload{ hdr, std::move(image), tile },
                    });
                };
            };

            auto [tile_w, tile_h] = ctx.tile_size;
            std::vector<task_type> tasks;
            for (auto& tile: tool::tile_iterator{tile_w, tile_h, view.size.x, view.size.y}) {
                tasks.emplace_back(make_task(false, 1, view, tile));
            }
            if (view.samples > 1) {
                for (auto& tile: tool::tile_iterator{tile_w, tile_h, view.size.x, view.size.y}) {
                    tasks.emplace_back(make_task(true, view.samples, view, tile));
                }
            }

            return tman.group(ctx.rx_gl.tx(), std::move(tasks));
        }

        tool::task_io pathtrace_combined_view_progressively(scene_type scene, view_type view, hdr_texture& hdr)
        {
            struct partial_result
            {
                std::mutex tile_mutex;
                image_rgb result;
                int samples{};

                partial_result(int w, int h): result{{w, h}} {}

                auto combine(image_rgb const& img, int img_samples) -> void
                {
                    auto old_samples = samples;
                    samples += img_samples;

                    result.each([&] (auto& color, auto pos) {
                        color *= old_samples;
                        color += img[pos];
                        color /= samples;
                    });
                }
            };
            using shared_partial_result = std::shared_ptr<partial_result>;

            struct tile_pos_less
            {
                auto operator () (tool::tile const& a, tool::tile const& b) const -> bool
                {
                    if (a.x < b.x) return true;
                    if (a.x > b.x) return false;
                    return (a.y < b.y);
                }
            };

            using task_type = tool::task_type<gl_job>;
            auto& ctx = context::instance();
            auto& tman = ctx.tman;
            auto shared_scene = std::make_shared<scene_type>(std::move(scene));
            shared_scene->rebuild_cache();

            auto make_task = [&hdr, &shared_scene] (auto& pres, int samples, auto view, auto tile) -> task_type {
                view.samples = samples;
                return [&hdr, view, tile, shared_scene, pres] (auto tx, auto shared_canceled) {
                    auto tile_lock = std::lock_guard{pres->tile_mutex};

                    tx.send(gl_job{
                        shared_canceled,
                        job::mark_as_working_tile{ hdr, tile },
                    });

                    pathtracer::pathtrace(
                        shared_canceled,
                        *shared_scene,
                        view,
                        tile,
                        [&] (auto& result) {
                            pres->combine(result, 1);
                            tx.send(gl_job{
                                shared_canceled,
                                job::upload{ hdr, pres->result, tile },
                            });
                            tx.send(gl_job{
                                shared_canceled,
                                job::mark_as_working_tile{ hdr, tile },
                            });
                        }
                    );

                    tx.send(gl_job{
                        shared_canceled,
                        job::upload{ hdr, pres->result, tile },
                    });
                };
            };

            auto make_tile_iter = [&] {
                auto [tile_w, tile_h] = ctx.tile_size;
                return tool::tile_iterator{tile_w, tile_h, view.size.x, view.size.y};
            };

            std::map<tool::tile, shared_partial_result, tile_pos_less> pres_per_tile;
            std::vector<task_type> tasks;

            for (auto& tile: make_tile_iter()) {
                pres_per_tile.emplace(tile, std::make_shared<partial_result>(tile.w, tile.h));
            }

            auto populate_layer_tasks = [&] (auto samples) {
                for (auto& tile: make_tile_iter()) {
                    tasks.emplace_back(make_task(pres_per_tile[tile], samples, view, tile));
                }
            };

            if (view.samples > 0) {
                auto samples_per_layer = ctx.batch_samples;

                populate_layer_tasks(1);
                auto remaining_samples = view.samples - 1;

                for (auto i=0; i<remaining_samples/samples_per_layer; i++)
                    populate_layer_tasks(samples_per_layer);
                remaining_samples %= samples_per_layer;

                if (remaining_samples > 0)
                    populate_layer_tasks(remaining_samples);
            }

            return tman.group(ctx.rx_gl.tx(), std::move(tasks));
        }

        tool::task_io swrast_combined_view_progressively(scene_type scene, view_type view, hdr_texture& hdr)
        {
            using task_type = tool::task_type<gl_job>;
            auto& ctx = context::instance();
            auto& tman = ctx.tman;
            auto tile = tool::tile{0, 0, view.size.x, view.size.y};

            scene.rebuild_cache();

            std::vector<task_type> tasks;
            tasks.emplace_back(
                [&hdr, tile, scene=std::move(scene), view=std::move(view)]
                (auto tx, auto shared_canceled) mutable {
                    auto image = swrast::rasterize(scene, std::move(view));

                    tx.send(gl_job{
                        shared_canceled,
                        job::upload{ hdr, std::move(image), tile },
                    });
                });

            return tman.group(ctx.rx_gl.tx(), std::move(tasks));
        }

        auto update_sk_visualization() -> void
        {
            auto& ctx = context::instance();
            auto& vi = *ctx.skein_visualization;
            vi.update_rasterization_state();
            vi.reset_raytracing_task_io();
            vi.reset_swrast_task_io();
            vi.suppress_raytracing = 10;
            vi.suppress_swrast = 1;
        }

        auto process_pending_gl_jobs() -> void
        {
            auto& ctx = context::instance();
            auto& rx = ctx.rx_gl;

            for (int i=0; i < ctx.frame_task_capacity; i++) {
                if (auto gl_job = rx.try_recv()) {
                    gl_job->run();
                } else {
                    break;
                }
            }
        }

        auto process_pending_dialog_jobs() -> void
        {
            auto& ctx = context::instance();

            while (auto message = ctx.skein_file_dialog_rx.try_recv()) {
                switch (message->serial) {
                    default: HEGEM_UNREACHABLE();
                    case context::skein_file_dialog_open_serial: ctx.skein_showing_open_file_dialog = false; break;
                    case context::skein_file_dialog_save_serial: ctx.skein_showing_save_file_dialog = false; break;
                };

                if (auto path = std::move(message->maybe_path); !path.empty()) {
                    switch (message->serial) {
                        default: HEGEM_UNREACHABLE();

                        case context::skein_file_dialog_open_serial: {
                            ctx.skein_editor.load_toml(path, ctx.skein_visualization->with_gizmo);
                            update_sk_visualization();
                            break;
                        }

                        case context::skein_file_dialog_save_serial: {
                            if (path.size() < 5 || path.substr(path.size() - 5) != ".toml") {
                                path += ".toml";
                            }
                            ctx.skein_editor.save_toml(path);
                            break;
                        }
                    };
                } else {
                    // User canceled. We carry out no action.
                }
            }
        }

        auto process_pending_jobs() -> void
        {
            process_pending_gl_jobs();
            process_pending_dialog_jobs();
        }

        namespace gui
        {
            auto adjustable_hdr_texture(hdr_texture& image, char const* label="adjustable hdr texture") -> void
            {
                ImGui::PushID(label);
                ImGui::PushItemWidth(-100);
                imgui_hdr_color("Blackpoint", "Intensity", &image.blackpoint, 0.001, -100, 10000, "%.2f", 10);
                imgui_hdr_color("Whitepoint", "Intensity", &image.whitepoint, 0.001, -100, 10000, "%.2f", 10);
                ImGui::SliderFloat("Dithering", &image.dither_amount, 0, 2, "%.2f");
                ImGui::PopItemWidth();
                ImGui::Separator();
                ImGui::BeginChild("image", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
                imgui_hdr_texture(&image);
                ImGui::EndChild();
                ImGui::PopID();
            }

            auto visualizer(visualization& vi) -> void
            {
                auto& ctx = context::instance();

                if (!vi.show_swrast_overlay) {
                    if (ImGui::Checkbox(vi.trace_path ? "Pathtrace" : "Raytrace", &vi.show_raytracing_overlay)) {
                        vi.reset_raytracing_task_io();
                        vi.suppress_raytracing = 1;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(vi.trace_path ? "Switch to Raytracing" : "Switch to Pathtracing")) {
                        vi.reset_raytracing_task_io();
                        vi.suppress_raytracing = 1;
                        vi.trace_path ^= true;
                    }
                }
                if (!vi.show_raytracing_overlay) {
                    if (!vi.show_swrast_overlay) ImGui::SameLine();
                    if (ImGui::Checkbox("Software Rasterize", &vi.show_swrast_overlay)) {
                        vi.reset_swrast_task_io();
                        vi.suppress_swrast = 1;
                    }
                }
                if (!vi.show_raytracing_overlay && !vi.show_swrast_overlay) {
                    ImGui::SameLine();
                    ImGui::Checkbox("Wireframed", &vi.wireframed);
                }

                ImGui::SameLine();
                if (ImGui::Checkbox("Gizmos", &vi.with_gizmo)) {
                    ctx.skein_editor.force_execute(vi.with_gizmo);
                    update_sk_visualization();
                    vi.reset_raytracing_task_io();
                    vi.reset_swrast_task_io();
                    vi.suppress_raytracing = 1;
                    vi.suppress_swrast = 1;
                }

                if (vi.hdr.dragging) {
                    ImGui::SameLine();
                    ImGui::Text("Dragging %.1f %.1f", vi.hdr.drag_offset.x, vi.hdr.drag_offset.y);
                    if (vi.show_raytracing_overlay) {
                        vi.reset_raytracing_task_io();
                        vi.suppress_raytracing = 10;
                    }
                    if (vi.show_swrast_overlay) {
                        vi.reset_swrast_task_io();
                        vi.suppress_swrast = 10;
                    }
                    vi.s.view.camera.match([&] (auto& cam) {
                        // TODO: make parameters adjustable
                        cam.tt.angles += vi.hdr.drag_offset * 0.01f;
                        cam.tt.zoffset = 5.0f;
                    });
                }
                if (vi.hdr.double_clicked) {
                    vi.show_raytracing_overlay = false;
                    vi.show_swrast_overlay = false;
                    vi.reset_raytracing_task_io();
                    vi.reset_swrast_task_io();
                    auto rvs = raytracer::raytrace(vi.s.scene, vi.s.view, vi.hdr.image_local_clicked_pos);
                    for (auto& rv: rvs)
                        vi.s.segments.emplace_back(rv.ray, rv.extent, rv.color, rv.width);
                }

                ImGui::Separator();
                ImGui::BeginChild("image viewer", ImVec2(0, 0), true);
                adjustable_hdr_texture(vi.hdr);
                if (ImGui::IsItemHovered()) {
                    auto wheel = ImGui::GetIO().MouseWheel;
                    if (wheel != 0.0f) {
                        vi.reset_raytracing_task_io();
                        vi.reset_swrast_task_io();
                        vi.suppress_raytracing = 10;
                        vi.suppress_swrast = 10;
                    }
                    vi.s.view.camera.match(
                        [&] (scene::cameras::pin_hole& cam) {
                            // TODO: make parameters adjustable
                            cam.fov -= wheel * 0.1f;
                            if (cam.fov < 0.1f) cam.fov = 0.1f;
                            if (cam.fov > 3.1f) cam.fov = 3.1f;
                        },
                        [&] (auto&) {});
                }
                ImGui::EndChild();

                if (vi.suppress_raytracing >= 0) vi.suppress_raytracing--;
                if (vi.suppress_raytracing == 0 && vi.show_raytracing_overlay) {
                    auto io = (
                        vi.trace_path
                        ? pathtrace_combined_view_progressively(vi.s.scene, vi.s.view, vi.hdr)
                        :  raytrace_combined_view_progressively(vi.s.scene, vi.s.view, vi.hdr)
                    );
                    vi.reset_raytracing_task_io(std::move(io));
                }

                if (vi.suppress_swrast >= 0) vi.suppress_swrast--;
                if (vi.suppress_swrast == 0 && vi.show_swrast_overlay) {
                    auto io = swrast_combined_view_progressively(vi.s.scene, vi.s.view, vi.hdr);
                    vi.reset_swrast_task_io(std::move(io));
                }

                if ((!vi.show_raytracing_overlay || vi.suppress_raytracing > 0)
                        && (!vi.show_swrast_overlay || vi.suppress_swrast > 0))
                    rasterizer::rasterize(vi.s, vi.wireframed, ImGui::GetTime());
            }

            auto hdr_viewer(int* selected) -> void
            {
                auto& ctx = context::instance();
                auto& images = ctx.images;
                static bool first_time = true;

                ImGui::Columns(2, "hdr viewer");
                if (first_time) {
                    first_time = false;
                    ImGui::SetColumnOffset(1, 300);
                }

                ImGui::BeginChild("image list", ImVec2(0, 0), true);
                for (int i=0; i < (int)images.size(); i++) {
                    ImGui::PushID(i);
                    if (ImGui::Selectable(images[i].name.data(), *selected == i))
                        *selected = i;
                    ImGui::PopID();
                }
                ImGui::EndChild();
                ImGui::NextColumn();

                ImGui::BeginChild("image viewer", ImVec2(0, 0), true);
                if (*selected >= 0 && *selected < (int)images.size()) {
                    adjustable_hdr_texture(images[*selected]);
                } else {
                    ImGui::Text("No image selected");
                }
                ImGui::EndChild();

                ImGui::Columns();
            }

            // returns true when "Render" is invoked
            template <class SceneList>
            bool scene_list(SceneList& scenes, ImGuiID* selected)
            {
                auto& ctx = context::instance();
                auto& vis = ctx.visualizations;
                bool render_invoked = false;
                static bool first_time = true;

                ImGui::Columns(6, "scene list");
                if (first_time) {
                    first_time = false;
                    ImGui::SetColumnWidth(0, 300);
                    ImGui::SetColumnWidth(2, 130);
                    ImGui::SetColumnWidth(3, 80);
                    ImGui::SetColumnWidth(4, 80);
                }
                ImGui::Text("Scene"); ImGui::NextColumn();
                ImGui::Text("View"); ImGui::NextColumn();
                ImGui::Text("Dimension"); ImGui::NextColumn();
                ImGui::Text("Bounces"); ImGui::NextColumn();
                ImGui::Text("Samples"); ImGui::NextColumn();
                ImGui::Text("Action"); ImGui::NextColumn();
                ImGui::Separator();

                int scene_idx = 0;
                for (auto& scene: scenes) {
                    ImGui::PushID(scene_idx++);
                    auto& scene_name = name(scene);

                    if (loaded(scene)) {
                        auto& loaded_scene = get_or_load(scene);
                        for (auto& view: loaded_scene.views) {
                            ImGui::PushID(view.name.data());
                            ImGui::AlignTextToFramePadding();
                            if (auto id = ImGui::GetID("selection");
                                    ImGui::Selectable(
                                        scene_name.data(),
                                        *selected == id)) {
                                *selected = id;
                            }
                            ImGui::NextColumn();
                            ImGui::AlignTextToFramePadding();
                            ImGui::Text("%s", view.name.data()); ImGui::NextColumn();

                            ImGui::PushItemWidth(-1);
                            ImGui::DragInt2("##dimension", &view.size[0], 0.1, 16, 1920);
                            ImGui::PopItemWidth();
                            ImGui::NextColumn();

                            ImGui::PushItemWidth(-1);
                            ImGui::DragInt("##bounces", &view.bounces, 0.1, 0, 16);
                            ImGui::PopItemWidth();
                            ImGui::NextColumn();

                            ImGui::PushItemWidth(-1);
                            ImGui::DragInt("##samples", &view.samples, 0.1, 1, 32);
                            ImGui::PopItemWidth();
                            ImGui::NextColumn();

                            if (ImGui::Button("Raytrace Channels")) {
                                render_view(loaded_scene, view);
                                render_invoked = true;
                            }
                            ImGui::SameLine(0.0f, 16.0f);
                            ImGui::TextUnformatted("Interactive");
                            ImGui::SameLine(0.0f, 4.0f);
                            if (ImGui::Button("Show")) {
                                vis.emplace_back(loaded_scene.name + ": " + view.name, loaded_scene, view, false);
                            }
                            ImGui::SameLine(0.0f, 2.0f);
                            if (ImGui::Button("Raytrace")) {
                                vis.emplace_back(loaded_scene.name + ": " + view.name, loaded_scene, view, true);
                            }
                            ImGui::SameLine(0.0f, 2.0f);
                            if (ImGui::Button("Pathtrace")) {
                                auto& vi = vis.emplace_back(loaded_scene.name + ": " + view.name, loaded_scene, view, true);
                                vi.trace_path = true;
                            }
                            ImGui::NextColumn();
                            ImGui::PopID();
                        }
                    } else {
                        if (auto id = ImGui::GetID("selection");
                                ImGui::Selectable(
                                    scene_name.data(),
                                    *selected == id)) {
                            *selected = id;
                        }
                        ImGui::NextColumn();
                        ImGui::TextUnformatted(""); ImGui::NextColumn();
                        ImGui::TextUnformatted(""); ImGui::NextColumn();
                        ImGui::TextUnformatted(""); ImGui::NextColumn();
                        ImGui::TextUnformatted(""); ImGui::NextColumn();
                        if (ImGui::Button("Load")) {
                            try {
                                get_or_load(scene);
                            }
                            catch (std::runtime_error const& e) {
                                j() << "\e[1;31merror: \e[0;31m" << e.what() << "\e[0m\n";
                            }
                        }
                        ImGui::NextColumn();
                    }

                    ImGui::PopID();
                }

                ImGui::Columns();
                return render_invoked;
            }

            auto framerates() -> void
            {
                auto& ctx = context::instance();
                auto avg_fps = ImGui::GetIO().Framerate;
                auto avg_frametime = 1000.0f / avg_fps;
                auto frametime = ImGui::GetIO().DeltaTime;
                auto fps = 1.0f / frametime;

                ctx.framerate_history[ctx.framerate_history_offset] = fps;
                ctx.framerate_history_offset = (ctx.framerate_history_offset + 1) % framerate_history_size;

                ImGui::Text("Average %.3f mspf, %.1f fps", avg_frametime, avg_fps);
                ImGui::PushItemWidth(-1);
                ImGui::PlotLines("##fps", ctx.framerate_history.data(), framerate_history_size, ctx.framerate_history_offset, nullptr, 0, 70, ImVec2(300, 80));
                ImGui::PopItemWidth();
            }

            template <class SceneList>
            auto main(SceneList& scenes) -> void
            {
                auto& ctx = context::instance();
                auto& images = ctx.images;
                auto& vis = ctx.visualizations;
                static bool show_demo_window = false;
                static bool show_scene_list = false;
                static bool show_hdr_viewer = false;
                static bool show_statistics = false;
                static bool show_framerates = false;
                static int selected_hdr_image = 0;
                static ImGuiID selected_scene_view = 0;

                ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Appearing);
                ImGui::SetNextWindowSize(ImVec2(330, 280), ImGuiCond_Appearing);
                ImGui::Begin("Options");
                if (ImGui::CollapsingHeader("Windows")) {
                    ImGui::Checkbox("Scenes", &show_scene_list);
                    ImGui::SameLine();
                    ImGui::Checkbox("HDR Viewer", &show_hdr_viewer);

                    ImGui::Checkbox("Statistics", &show_statistics);
                    ImGui::SameLine();
                    ImGui::Checkbox("Framerates", &show_framerates);

                    ImGui::Separator();
                    ImGui::Checkbox("ImGui Demo", &show_demo_window);
                }
                if (ImGui::CollapsingHeader("Scheduling Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::PushItemWidth(-130);
                    ImGui::DragInt2("Tile Size", ctx.tile_size, 0.1, 4, 512);
                    ImGui::DragInt2("Tile Size (Morphing)", ctx.morphing_tile_size, 0.1, 256, 2048);
                    ImGui::DragInt("Batch Samples", &ctx.batch_samples, 0.1, 2, 512);
                    ImGui::DragInt("Tasks per Frame", &ctx.frame_task_capacity, 0.1, 16, 512);
                    if (ctx.batch_samples < 2) ctx.batch_samples = 2;
                    ImGui::PopItemWidth();
                }
                if (ImGui::CollapsingHeader("Editor", ImGuiTreeNodeFlags_DefaultOpen)) {
                    if (ctx.skein_showing_open_file_dialog) {
                        ImGui::AlignTextToFramePadding();
                        ImGui::TextDisabled("Open");
                    } else {
                        if (ImGui::Button("Open")) {
                            ctx.skein_showing_open_file_dialog = true;
                            tool::show_open_file_dialog(&ctx.desktop, ctx.skein_file_dialog_rx.tx(), context::skein_file_dialog_open_serial, "Open skein graph", "/usr/share/hegem/support/graph");
                        }
                    }
                    ImGui::SameLine();
                    if (ctx.skein_showing_save_file_dialog) {
                        ImGui::TextDisabled("Save");
                    } else {
                        if (ImGui::Button("Save")) {
                            ctx.skein_showing_save_file_dialog = true;
                            tool::show_save_file_dialog(&ctx.desktop, ctx.skein_file_dialog_rx.tx(), context::skein_file_dialog_save_serial, "Save skein graph");
                        }
                    }
                }

                ImGui::End();

                if (show_demo_window) {
                    ImGui::SetNextWindowPos(ImVec2(50, 440), ImGuiCond_Appearing);
                    ImGui::ShowDemoWindow(&show_demo_window);
                }

                if (show_scene_list) {
                    ImGui::SetNextWindowPos(ImVec2(50, 200), ImGuiCond_Appearing);
                    ImGui::SetNextWindowSize(ImVec2(1200, 200), ImGuiCond_FirstUseEver);
                    ImGui::Begin("Scenes", &show_scene_list);
                    if (scene_list(scenes, &selected_scene_view)) {
                        selected_hdr_image = images.size() - 1;
                        show_hdr_viewer = true;
                    }
                    ImGui::End();
                }

                if (show_hdr_viewer) {
                    ImGui::SetNextWindowPos(ImVec2(300, 50), ImGuiCond_Appearing);
                    ImGui::SetNextWindowSize(ImVec2(1300, 800), ImGuiCond_FirstUseEver);
                    ImGui::Begin("HDR Viewer", &show_hdr_viewer);
                    hdr_viewer(&selected_hdr_image);
                    ImGui::End();
                }

                if (ctx.skein_visualization == nullptr || !ctx.skein_visualization->show) {
                    auto& scene = ctx.skein_editor.scene;
                    auto& view = scene.views.front();
                    ctx.skein_visualization = &vis.emplace_back("Preview", scene, view, false);
                    ctx.skein_editor.force_execute(ctx.skein_visualization->with_gizmo);
                    update_sk_visualization();
                }

                vis.remove_if([] (auto& vi) { return !vi.show; });
                int vi_idx = 0;
                for (auto& vi: vis) {
                    ImGui::SetNextWindowPos(ImVec2(50, 300), ImGuiCond_Appearing);
                    ImGui::SetNextWindowSize(ImVec2(1000, 800), ImGuiCond_FirstUseEver);
                    auto name = vi.name + "##" + std::to_string(vi_idx++);
                    ImGui::Begin(name.data(), &vi.show);
                    visualizer(vi);
                    ImGui::End();
                    if (!vi.show) {
                        vi.reset_raytracing_task_io();
                        vi.reset_swrast_task_io();
                    }
                }

                if (show_framerates) {
                    ImGui::SetNextWindowPos(ImVec2(50, 500), ImGuiCond_Appearing);
                    ImGui::Begin("Framerates", &show_framerates, ImGuiWindowFlags_AlwaysAutoResize);
                    framerates();
                    ImGui::End();
                }

                if (show_statistics) {
                    ImGui::SetNextWindowPos(ImVec2(50, 600), ImGuiCond_FirstUseEver);
                    ImGui::SetNextWindowSize(ImVec2(425, 295), ImGuiCond_FirstUseEver);
                    ImGui::Begin("Statistics", &show_statistics);
                    view::statistics("statistics");
                    ImGui::End();
                }

                ImGui::SetNextWindowPos(ImVec2{0.0f, 0.0f});
                ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
                auto& style = ImGui::GetStyle();
                auto padding = style.WindowPadding;
                style.WindowPadding = ImVec2{0.0f, 0.0f};
                ImGui::Begin("Sk Editor", nullptr,
                    ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove |
                    ImGuiWindowFlags_NoScrollbar |
                    ImGuiWindowFlags_NoBringToFrontOnFocus);
                style.WindowPadding = padding;
                if (ctx.skein_editor.draw(ctx.skein_visualization->with_gizmo))
                    update_sk_visualization();
                ImGui::End();

                ImGui::SetNextWindowPos(ImVec2(400, 50), ImGuiCond_Appearing);
                ImGui::SetNextWindowSize(ImVec2(1000, 800), ImGuiCond_FirstUseEver);
                ImGui::Begin("Silo - The Morphing Editor");
                ctx.silo_editor();
                ImGui::End();

                process_pending_jobs();
            }
        }
    }

    auto run_once(options opts) -> void
    {
        j() << "run\n";
        glfw::init_once("Hegem");
        glu::init_all_resource_pools_once();

        glfw::mainloop_once([&] () {
            glu::states_manager::instance().enable_only({});
            gl::clear_bufferfv(gl::color, 0, background);
            gui::main(opts.scenes);
            glu::resource_recycler::instance().try_recycle();
        });
    }

    auto schedule_tasks(task_group_type tasks) -> tool::task_io
    {
        auto& ctx = context::instance();
        auto& tman = ctx.tman;
        return tman.group(ctx.rx_gl.tx(), std::move(tasks));
    }
}

