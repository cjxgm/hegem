#include "../lib/imgui.hh"
#include "../scene/parser.hh"
#include "../scene/view.hh"
#include "../image/image.hh"
#include "../util/journal.hh"
#include "../util/tile.hh"
#include "../util/scheduler.hh"
#include "../util/file-dialog.hh"
#include "../glu/states.hh"
#include "../raytracer/raytracer.hh"
#include "../raytracer/shade.hh"
#include "../pathtracer/pathtracer.hh"
#include "../rasterizer/rasterizer.hh"
#include "../rasterizer/state.hh"
#include "../swrast/rasterizer.hh"
#include "../sk/editor.hh"
#include "app.hh"
#include "glfw.hh"
#include "hdr-texture.hh"
#include "visualization.hh"
#include "statistics.hh"
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

namespace rt::app
{
    namespace
    {
        using util::journal;
        using image::image_rgb;
        using image::color::linear_rgb;
        using scene::scene_type;
        using scene::view_type;

        journal j() { return {"APP"}; }

        // Process at most `frame_task_capacity` number of tasks per frame
        static constexpr auto framerate_history_size = 60*2;
        float const background[] = { 0.2667, 0.5333, 1.0000, 0.0000 };

        using gl_job = util::possibly_canceled_job;

        struct context
        {
            std::deque<hdr_texture> images;
            std::list<visualization> visualizations;
            util::receiver<gl_job> rx_gl;
            util::task_manager<util::pool_scheduler> tman{util::pool_scheduler{(int) std::max(4u, std::thread::hardware_concurrency())}};
            int tile_size[2] = {64, 64};
            int morphing_tile_size[2] = {128, 128};
            int batch_samples = 16;
            int frame_task_capacity = 64;
            std::array<float, framerate_history_size> framerate_history{};
            int framerate_history_offset{};
            sk::editor sk_editor;
            visualization* sk_visualization{};
            util::file_dialog sk_file_open_dialog;
            util::file_dialog sk_file_save_dialog;

            static context& instance()
            {
                static context ctx;
                return ctx;
            }

            ~context()
            {
                j() << "context: (dtor)\n";
                j() << "canceling raytracing and swrast process...\n";
                for (auto& vi: visualizations) {
                    vi.reset_raytracing_task_io();
                    vi.reset_swrast_task_io();
                }
            }

        private:
            context()
            {
                j() << "context: (ctor)\n";
            }
        };

        namespace job
        {
            struct upload
            {
                hdr_texture& hdr;
                image_rgb image;
                util::tile tile;

                void operator () () const
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
                util::tile tile;

                void operator () () const
                {
                    hdr.mark(tile);
                }
            };
        }

        util::task_io render_view(scene_type const& scene, view_type view)
        {
            using task_type = util::task_type<gl_job>;
            auto& ctx = context::instance();
            auto& images = ctx.images;
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

                    auto result = raytracer::raytrace(scene, view, tile);
                    auto& image = std::get<0>(result);
                    auto& buf = std::get<1>(result);

                    send_job(job::upload{ hdr_combined, std::move(image), tile });

                    auto depth = raytracer::shade_depth(buf, view);
                    send_job(job::upload{ hdr_depth, std::move(depth), tile });

                    auto normal = raytracer::shade_normal(buf, view);
                    send_job(job::upload{ hdr_normal, std::move(normal), tile });
                };
            };

            auto [tile_w, tile_h] = ctx.tile_size;
            std::vector<task_type> tasks;
            for (auto& tile: util::tile_iterator{tile_w, tile_h, view.size.x, view.size.y}) {
                tasks.emplace_back(make_task(view, tile));
            }

            return tman.group(ctx.rx_gl.tx(), std::move(tasks));
        }

        util::task_io raytrace_combined_view_progressively(scene_type scene, view_type view, hdr_texture& hdr)
        {
            using task_type = util::task_type<gl_job>;
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

                    auto [image, _] = raytracer::raytrace(*shared_scene, view, tile);
                    (void) _;

                    tx.send(gl_job{
                        shared_canceled,
                        job::upload{ hdr, std::move(image), tile },
                    });
                };
            };

            auto [tile_w, tile_h] = ctx.tile_size;
            std::vector<task_type> tasks;
            for (auto& tile: util::tile_iterator{tile_w, tile_h, view.size.x, view.size.y}) {
                tasks.emplace_back(make_task(false, 1, view, tile));
            }
            if (view.samples > 1) {
                for (auto& tile: util::tile_iterator{tile_w, tile_h, view.size.x, view.size.y}) {
                    tasks.emplace_back(make_task(true, view.samples, view, tile));
                }
            }

            return tman.group(ctx.rx_gl.tx(), std::move(tasks));
        }

        util::task_io pathtrace_combined_view_progressively(scene_type scene, view_type view, hdr_texture& hdr)
        {
            struct partial_result
            {
                std::mutex tile_mutex;
                image_rgb result;
                int samples{};

                partial_result(int w, int h): result{{w, h}} {}

                void combine(image_rgb const& img, int img_samples)
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
                auto operator () (util::tile const& a, util::tile const& b) const -> bool
                {
                    if (a.x < b.x) return true;
                    if (a.x > b.x) return false;
                    return (a.y < b.y);
                }
            };

            using task_type = util::task_type<gl_job>;
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
                        });

                    tx.send(gl_job{
                        shared_canceled,
                        job::upload{ hdr, pres->result, tile },
                    });
                };
            };

            auto make_tile_iter = [&] {
                auto [tile_w, tile_h] = ctx.tile_size;
                return util::tile_iterator{tile_w, tile_h, view.size.x, view.size.y};
            };

            std::map<util::tile, shared_partial_result, tile_pos_less> pres_per_tile;
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

        util::task_io swrast_combined_view_progressively(scene_type scene, view_type view, hdr_texture& hdr)
        {
            using task_type = util::task_type<gl_job>;
            auto& ctx = context::instance();
            auto& tman = ctx.tman;
            auto tile = util::tile{0, 0, view.size.x, view.size.y};

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

        void update_sk_visualization()
        {
            auto& ctx = context::instance();
            auto& vi = *ctx.sk_visualization;
            vi.update_rasterization_state();
            vi.reset_raytracing_task_io();
            vi.reset_swrast_task_io();
            vi.suppress_raytracing = 10;
            vi.suppress_swrast = 1;
        }

        void process_pending_gl_jobs()
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

        void process_pending_dialog_jobs()
        {
            auto& ctx = context::instance();

            if (auto opt_path = ctx.sk_file_save_dialog()) {
                auto path = std::move(*opt_path);
                if (path.size() < 5 || path.substr(path.size() - 5) != ".toml")
                    path += ".toml";
                ctx.sk_editor.save_toml(path);
            }

            if (auto opt_path = ctx.sk_file_open_dialog()) {
                auto path = std::move(*opt_path);
                ctx.sk_editor.load_toml(path, ctx.sk_visualization->with_gizmo);
                update_sk_visualization();
            }
        }

        void process_pending_jobs()
        {
            process_pending_gl_jobs();
            process_pending_dialog_jobs();
        }

        namespace gui
        {
            void adjustable_hdr_texture(hdr_texture& image, char const* label="adjustable hdr texture")
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

            void visualizer(visualization& vi)
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
                    ctx.sk_editor.force_execute(vi.with_gizmo);
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

            void hdr_viewer(int* selected)
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
                    ImGui::SetColumnWidth(3, 100);
                    ImGui::SetColumnWidth(4, 100);
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

                            if (ImGui::Button("Render")) {
                                render_view(loaded_scene, view);
                                render_invoked = true;
                            }
                            ImGui::SameLine();
                            if (ImGui::Button("Visualize")) {
                                vis.emplace_back(loaded_scene.name + ": " + view.name, loaded_scene, view, false);
                            }
                            ImGui::SameLine();
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

            void framerates()
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
            void main(SceneList& scenes)
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
                    if (ctx.sk_file_open_dialog) {
                        ImGui::AlignTextToFramePadding();
                        ImGui::TextDisabled("Open");
                    } else {
                        if (ImGui::Button("Open")) {
                            ctx.sk_file_open_dialog.open(
                                util::file_dialog::action::open,
                                "Open stack graph",
                                "/usr/share/hegem/support/graph");
                        }
                    }
                    ImGui::SameLine();
                    if (ctx.sk_file_save_dialog) {
                        ImGui::TextDisabled("Save");
                    } else {
                        if (ImGui::Button("Save")) {
                            ctx.sk_file_save_dialog.open(
                                util::file_dialog::action::save,
                                "Save stack graph");
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

                if (ctx.sk_visualization == nullptr || !ctx.sk_visualization->show) {
                    auto& scene = ctx.sk_editor.scene;
                    auto& view = scene.views.front();
                    ctx.sk_visualization = &vis.emplace_back("Hegem Preview", scene, view, false);
                    ctx.sk_editor.force_execute(ctx.sk_visualization->with_gizmo);
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
                if (ctx.sk_editor.draw(ctx.sk_visualization->with_gizmo))
                    update_sk_visualization();
                ImGui::End();

                process_pending_jobs();
            }
        }
    }

    void run_once(options opts)
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

    auto schedule_tasks(task_group_type tasks) -> util::task_io
    {
        auto& ctx = context::instance();
        auto& tman = ctx.tman;
        return tman.group(ctx.rx_gl.tx(), std::move(tasks));
    }
}

