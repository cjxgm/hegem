#include "../lib/gl/gl.hh"
#include "../lib/imgui.hh"
#include "../util/journal.hh"
#include "../util/mpsc.hh"
#include "../app/scene-instance.hh"
#include "../image/image.hh"
#include "../raytracer/raytracer.hh"
#include "../raytracer/shade.hh"
#include "ui.hh"
#include "wsi.hh"
#include "hdr-texture.hh"
#include <deque>
#include <thread>
#include <functional>

using rt::util::journal;

namespace rt::gui
{
    namespace
    {
        using image::image_rgb;
        using scene::scene_type;

        journal j() { return {"UI"}; }

        std::deque<hdr_texture> images;
        util::mpsc<std::function<void()>> tasks;

        struct upload_job
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

        void render_job(scene_type const& scene, int view_id, hdr_texture const& hdr)
        {
            auto result = raytracer::raytrace(scene, view_id, 8);
            auto& image = std::get<0>(result);
            tasks.push(upload_job{ hdr, std::move(image) });
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

        void upload_render_results()
        {
            while (tasks) {
                auto task = tasks.pop();
                task();
            }
        }
    }

    ui::ui()
    {
        j() << "ui: (ctor)\n";
    }

    ui::~ui()
    {
        j() << "ui: (dtor)\n";
    }

    void ui::render()
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
            render_all_views(app::scene_instance());
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

        upload_render_results();
    }
}

