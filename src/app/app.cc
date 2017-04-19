#include "../lib/imgui.hh"
#include "../scene/parser.hh"
#include "../scene/view.hh"
#include "../image/image.hh"
#include "../util/journal.hh"
#include "../util/mpsc.hh"
#include "../raytracer/raytracer.hh"
#include "app.hh"
#include "glfw.hh"
#include "hdr-texture.hh"
#include <deque>
#include <thread>
#include <functional>

namespace rt::app
{
    namespace
    {
        using util::journal;
        using image::image_rgb;
        using image::color::linear_rgb;
        using scene::scene_type;

        journal j() { return {"APP"}; }

        std::deque<hdr_texture> images;
        util::mpsc<std::function<void()>> tasks;

        namespace job
        {
            struct upload
            {
                hdr_texture const& hdr;
                image_rgb image;

                void operator () () const
                {
                    auto size = image.size();
                    gl::texture_sub_image2d(
                            hdr.tex.get(),
                            0,
                            0, 0, size.x, size.y,
                            gl::rgb, gl::float_,
                            image.data());
                }
            };
        }

        void render_job(scene_type const& scene, int view_id, hdr_texture const& hdr)
        {
            auto result = raytracer::raytrace(scene, view_id, 8);
            auto& image = std::get<0>(result);
            tasks.push(job::upload{ hdr, std::move(image) });
        }

        void render_view(scene_type const& scene, int view_id)
        {
            auto& view = scene.views[view_id];
            images.emplace_back(view.size().x, view.size().y);

            auto& hdr = images.back();
            gl::texture_parameteri(hdr.tex.get(), gl::texture_min_filter, gl::linear);
            gl::texture_parameteri(hdr.tex.get(), gl::texture_mag_filter, gl::nearest);

            std::thread{render_job, std::cref(scene), view_id, std::cref(hdr)}.detach();
        }

        void render_all_views(scene_type const& scene)
        {
            for (auto i=0u; i < scene.views.size(); i++)
                render_view(scene, i);
        }

        void process_pending_tasks()
        {
            for (int i=0; i < 32 && tasks; i++) {
                auto task = tasks.pop();
                task();
            }
        }

        void render_gui(scene_type const& scene)
        {
            static float background[] = {0.2667, 0.5333, 1.0000, 0.0000};
            gl::clear_bufferfv(gl::color, 0, background);

            static bool show_test_window = false;
            static int selected_render_image = 0;

            ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Appearing);
            ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Appearing);
            ImGui::Begin("Control");
            if (ImGui::Button("Render")) {
                selected_render_image = images.size();
                render_all_views(scene);
            }
            ImGui::Separator();
            if (ImGui::Button("ImGui Demo")) show_test_window ^= 1;
            ImGui::End();

            if (show_test_window) {
                ImGui::SetNextWindowPos(ImVec2(300, 20), ImGuiSetCond_Appearing);
                ImGui::ShowTestWindow(&show_test_window);
            }

            if (images.size()) {
                ImGui::SetNextWindowPos(ImVec2(250, 20), ImGuiSetCond_Appearing);
                ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiSetCond_Appearing);
                ImGui::Begin("Image Viewer");

                ImGui::BeginChild("image selector", ImVec2(150, 0), true);
                for (int i=0; i < (int)images.size(); i++) {
                    auto label = "Image " + std::to_string(i);
                    if (ImGui::Selectable(label.data(), selected_render_image == i))
                        selected_render_image = i;
                }
                ImGui::EndChild();
                ImGui::SameLine();

                ImGui::BeginChild("image viewer", ImVec2(0, 0), true);
                if (selected_render_image >= 0 && selected_render_image < (int)images.size()) {
                    auto& image = images[selected_render_image];
                    imgui_hdr_color("Blackpoint", "Intensity", &image.blackpoint, 0.001, -100, 10000, "%.2f", 10);
                    imgui_hdr_color("Whitepoint", "Intensity", &image.whitepoint, 0.001, -100, 10000, "%.2f", 10);
                    ImGui::BeginChild("image viewer", ImVec2(0, 0), false);
                    imgui_hdr_texture(&image);
                    ImGui::EndChild();
                } else {
                    ImGui::Text("No image selected");
                }
                ImGui::EndChild();

                ImGui::End();
            }

            process_pending_tasks();
        }
    }

    void run_once(options const& opts)
    {
        j() << "loading scene\n";
        auto s = scene::from_path(opts.input_path);
        s.views[0] = scene::view_type {
            {opts.width, opts.height},
            s.views[0].camera(),
        };

        glfw::init_once("Raytracer");
        glfw::mainloop_once([&] () { render_gui(s); });

        #if 0
        auto result = raytracer::raytrace(s, 0, 8);
        auto& image = std::get<0>(result);
        auto& buf = std::get<1>(result);

        if (!opts.output_path.empty()) {
            write(to_srgb(tonemap(
                            image,
                            linear_rgb{0},
                            linear_rgb{10})),
                    opts.output_path);
        }
        if (!opts.depth_path.empty()) {
            auto depth = raytracer::shade_depth(buf, s, 0);
            write(to_srgb(tonemap(
                            depth,
                            linear_rgb{opts.depth_max},
                            linear_rgb{opts.depth_min})),
                    opts.depth_path);
        }
        if (!opts.normal_path.empty()) {
            auto normal = raytracer::shade_normal(buf, s, 0);
            write(to_srgb(normal), opts.normal_path);
        }
        #endif
    }
}

