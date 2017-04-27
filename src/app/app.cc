#include "../lib/imgui.hh"
#include "../scene/parser.hh"
#include "../scene/view.hh"
#include "../image/image.hh"
#include "../util/journal.hh"
#include "../util/mpsc.hh"
#include "../util/tile.hh"
#include "../util/spawn.hh"
#include "../glu/states.hh"
#include "../raytracer/raytracer.hh"
#include "../raytracer/shade.hh"
#include "app.hh"
#include "glfw.hh"
#include "hdr-texture.hh"
#include <deque>
#include <array>
#include <functional>

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
        static constexpr auto frame_task_capacity = 32;
        static constexpr auto framerate_history_size = 60*2;
        float const background[] = { 0.2667, 0.5333, 1.0000, 0.0000 };
        //float const working_tile_border[] = { 10, 6, 1, 1 };
        //float const working_tile_background[] = { 0, 0, 0, 1 };

        struct context
        {
            std::deque<hdr_texture> images;
            util::mpsc<std::function<void()>> tasks;
            int tile_size[2] = {64, 64};
            std::array<float, framerate_history_size> framerate_history{};
            int framerate_history_offset{};

            static context& instance()
            {
                static context ctx;
                return ctx;
            }

            ~context()
            {
                j() << "context: (dtor)\n";
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
                hdr_texture const& hdr;
                image_rgb image;
                util::tile tile;

                void operator () () const
                {
                    gl::texture_sub_image2d(
                            hdr.tex.get(),
                            0,
                            tile.x, tile.y, tile.w, tile.h,
                            gl::rgb, gl::float_,
                            image.data());
                }
            };

            // FIXME: This job causes heavy framedrops.
            struct mark_as_working_tile
            {
                hdr_texture const& hdr;
                util::tile tile;

                void operator () () const
                {
                    //j() << "working on " << hdr.name
                    //    << " texture " << (int)hdr.tex.get()
                    //    << " tile (" << tile.x << ", " << tile.y << ") "
                    //    << tile.w << "x" << tile.h
                    //    << "\n";
                    //gl::clear_tex_sub_image(
                    //        hdr.tex.get(),
                    //        0,
                    //        tile.x, tile.y, 0, tile.w, tile.h, 1,
                    //        gl::rgba, gl::float_,
                    //        working_tile_border);
                    //if (tile.w > 2 && tile.h > 2) {
                    //    gl::clear_tex_sub_image(
                    //            hdr.tex.get(),
                    //            0,
                    //            tile.x+1, tile.y+1, 0, tile.w-2, tile.h-2, 1,
                    //            gl::rgba, gl::float_,
                    //            working_tile_background);
                    //}
                }
            };
        }

        void render_job(
                scene_type const& scene,
                view_type view,
                util::tile tile,
                hdr_texture const& hdr_combined,
                hdr_texture const& hdr_depth,
                hdr_texture const& hdr_normal)
        {
            auto& ctx = context::instance();
            auto& tasks = ctx.tasks;

            tasks.push(job::mark_as_working_tile{ hdr_combined, tile });
            tasks.push(job::mark_as_working_tile{ hdr_depth, tile });
            tasks.push(job::mark_as_working_tile{ hdr_normal, tile });

            auto result = raytracer::raytrace(scene, view, tile);
            auto& image = std::get<0>(result);
            auto& buf = std::get<1>(result);

            tasks.push(job::upload{ hdr_combined, std::move(image), tile });

            auto depth = raytracer::shade_depth(buf, view);
            tasks.push(job::upload{ hdr_depth, std::move(depth), tile });

            auto normal = raytracer::shade_normal(buf, view);
            tasks.push(job::upload{ hdr_normal, std::move(normal), tile });
        }

        void render_view(scene_type const& scene, view_type view, util::spawner& spawner)
        {
            auto& ctx = context::instance();
            auto& images = ctx.images;
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

            auto [tile_w, tile_h] = ctx.tile_size;
            for (auto& tile: util::tile_iterator{tile_w, tile_h, view.size.x, view.size.y}) {
                spawner.spawn([&, view, tile] () {
                    render_job(scene, view, tile, hdr_combined, hdr_depth, hdr_normal);
                });
            }
        }

        void process_pending_tasks()
        {
            auto& ctx = context::instance();
            auto& tasks = ctx.tasks;

            for (int i=0; i < frame_task_capacity && tasks; i++) {
                auto task = tasks.pop();
                task();
            }
        }

        namespace gui
        {
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
                    auto& image = images[*selected];
                    ImGui::PushItemWidth(-100);
                    imgui_hdr_color("Blackpoint", "Intensity", &image.blackpoint, 0.001, -100, 10000, "%.2f", 10);
                    imgui_hdr_color("Whitepoint", "Intensity", &image.whitepoint, 0.001, -100, 10000, "%.2f", 10);
                    ImGui::SliderFloat("Dithering", &image.dither_amount, 0, 2, "%.2f");
                    ImGui::PopItemWidth();
                    ImGui::Separator();
                    ImGui::BeginChild("image", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
                    imgui_hdr_texture(&image);
                    ImGui::EndChild();
                } else {
                    ImGui::Text("No image selected");
                }
                ImGui::EndChild();

                ImGui::Columns();
            }

            // returns true when "Render" is invoked
            template <class SceneList>
            bool scene_list(SceneList& scenes, ImGuiID* selected, util::spawner& spawner)
            {
                bool render_invoked = false;
                static bool first_time = true;

                ImGui::Columns(5, "scene list");
                if (first_time) {
                    first_time = false;
                    ImGui::SetColumnOffset(1, 300);
                }
                ImGui::Text("Scene"); ImGui::NextColumn();
                ImGui::Text("View"); ImGui::NextColumn();
                ImGui::Text("Dimension"); ImGui::NextColumn();
                ImGui::Text("Bounces"); ImGui::NextColumn();
                ImGui::Text("Action"); ImGui::NextColumn();
                ImGui::Separator();

                for (auto& scene: scenes) {
                    auto& scene_name = name(scene);
                    ImGui::PushID(scene_name.data());

                    if (loaded(scene)) {
                        auto& loaded_scene = get_or_load(scene);
                        for (auto& view: loaded_scene.views) {
                            ImGui::PushID(view.name.data());
                            if (auto id = ImGui::GetID("selection");
                                    ImGui::Selectable(
                                        scene_name.data(),
                                        *selected == id)) {
                                *selected = id;
                            }
                            ImGui::NextColumn();
                            ImGui::Text("%s", view.name.data()); ImGui::NextColumn();

                            ImGui::PushItemWidth(-1);
                            ImGui::DragInt2("##dimension", &view.size[0], 0.1, 16, 1920);
                            ImGui::PopItemWidth();
                            ImGui::NextColumn();

                            ImGui::PushItemWidth(-1);
                            ImGui::DragInt("##bounces", &view.bounces, 0.1, 0, 16);
                            ImGui::PopItemWidth();
                            ImGui::NextColumn();

                            if (ImGui::Button("Render")) {
                                render_view(loaded_scene, view, spawner);
                                render_invoked = true;
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
                        ImGui::Text(""); ImGui::NextColumn();
                        ImGui::Text(""); ImGui::NextColumn();
                        ImGui::Text(""); ImGui::NextColumn();
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
            void main(SceneList& scenes, util::spawner& spawner)
            {
                auto& ctx = context::instance();
                auto& images = ctx.images;
                static bool show_test_window = false;
                static bool show_scene_list = true;
                static bool show_hdr_viewer = false;
                static int selected_hdr_image = 0;
                static ImGuiID selected_scene_view = 0;

                ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiSetCond_Appearing);
                ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiSetCond_Appearing);
                ImGui::Begin("Control");
                if (ImGui::Button("Scenes")) show_scene_list ^= 1;
                ImGui::SameLine();
                if (ImGui::Button("HDR Viewer")) show_hdr_viewer ^= 1;
                ImGui::Separator();
                ImGui::PushItemWidth(-100);
                ImGui::DragInt2("Tile Size", ctx.tile_size, 0.1, 4, 512);
                ImGui::PopItemWidth();
                ImGui::Separator();
                if (ImGui::Button("ImGui Demo")) show_test_window ^= 1;
                ImGui::End();

                if (show_test_window) {
                    ImGui::SetNextWindowPos(ImVec2(50, 440), ImGuiSetCond_Appearing);
                    ImGui::ShowTestWindow(&show_test_window);
                }

                if (show_scene_list) {
                    ImGui::SetNextWindowPos(ImVec2(50, 200), ImGuiSetCond_Appearing);
                    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);
                    ImGui::Begin("Scenes", &show_scene_list);
                    if (scene_list(scenes, &selected_scene_view, spawner)) {
                        selected_hdr_image = images.size() - 1;
                        show_hdr_viewer = true;
                    }
                    ImGui::End();
                }

                if (show_hdr_viewer) {
                    ImGui::SetNextWindowPos(ImVec2(300, 50), ImGuiSetCond_Appearing);
                    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiSetCond_FirstUseEver);
                    ImGui::Begin("HDR Viewer", &show_hdr_viewer);
                    hdr_viewer(&selected_hdr_image);
                    ImGui::End();
                }

                ImGui::SetNextWindowPos(ImVec2(50, 500), ImGuiSetCond_Appearing);
                ImGui::Begin("Framerates", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
                framerates();
                ImGui::End();

                process_pending_tasks();
            }
        }
    }

    void run_once(options opts)
    {
        j() << "run\n";

        util::spawner spawner{4};   // TODO: auto detect threads? allow customization?
        glfw::init_once("Raytracer");
        glfw::mainloop_once([&] () {
            glu::states_manager::instance().enable_only({});
            gl::clear_bufferfv(gl::color, 0, background);
            gui::main(opts.scenes, spawner);
            glu::resource_recycler::instance().try_recycle();
        });
    }
}

