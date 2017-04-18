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
#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include <deque>
#include <thread>

using rt::util::journal;

namespace rt::gui
{
    namespace
    {
        using image::image_rgb;
        using scene::scene_type;

        journal j() { return {"UI"}; }

        struct texture2d
        {
            texture2d(int w, int h, int lod=1)
                : w{w}, h{h}
            {
                gl::create_textures(gl::texture_2d, 1, &tex);
                gl::texture_storage2d(tex, lod, gl::rgba32f, w, h);
                unique_tex.reset(&tex);
            }

            gl::uint_type & get() { return tex; }
            gl::uint_type const& get() const { return tex; }
            gl::uint_type const& operator * () const { return get(); }

            auto width() const { return w; }
            auto height() const { return h; }

        private:
            int w;
            int h;
            gl::uint_type tex;

            struct deleter_type
            {
                using pointer = void*;
                void operator () (pointer p)
                {
                    gl::delete_textures(1, static_cast<gl::uint_type*>(p));
                }
            };

            std::unique_ptr<void, deleter_type> unique_tex{};
        };

        struct render_result
        {
            gl::uint_type tex;
            image_rgb image;
        };

        std::vector<texture2d> images;
        std::atomic_int render_job_count{};
        util::mpsc<render_result> render_results;

        void render_view(scene_type const& scene, int view_id)
        {
            auto& view = scene.views[view_id];
            images.emplace_back(view.size().x, view.size().y);

            auto tex = images.back().get();
            gl::texture_parameteri(tex, gl::texture_min_filter, gl::linear);
            gl::texture_parameteri(tex, gl::texture_mag_filter, gl::nearest);

            render_job_count++;
            std::thread{[&scene, view_id, tex] () {
                auto result = raytracer::raytrace(scene, view_id, 8);
                auto& image = std::get<0>(result);
                render_job_count--;
                render_results.push({ tex, std::move(image) });
            }}.detach();
        }

        void render_all_views(scene_type const& scene)
        {
            for (auto i=0u; i < scene.views.size(); i++)
                render_view(scene, i);
        }

        void upload_render_results()
        {
            while (render_results) {
                auto result = render_results.pop();
                auto size = result.image.size();
                gl::texture_sub_image2d(
                        result.tex,
                        0,
                        0, 0, size.x, size.y,
                        gl::rgb, gl::float_,
                        result.image.data());
            }
        }

        struct tonemapping_data
        {
            float min;
            float max;
        };

        void tonemapping_command(ImDrawList const* /*pdraw_list*/, ImDrawCmd const* pcmd)
        {
            auto pdata = static_cast<tonemapping_data*>(pcmd->UserCallbackData);
            gl::uniform1i(2, 1);                        // tonemap mode
            gl::uniform2f(3, pdata->min, pdata->max);   // tonemap range
            delete pdata;
        }

        void tonemapping_restore(ImDrawList const* /*pdraw_list*/, ImDrawCmd const* /*pcmd*/)
        {
            gl::uniform1i(2, 0);                        // general mode
        }

        void begin_tonemapping(float min, float max)
        {
            auto& cmd_list = *ImGui::GetWindowDrawList();
            // FIXME: needs an allocator
            auto data = new tonemapping_data{min, max};
            cmd_list.AddDrawCmd();
            cmd_list.AddCallback(tonemapping_command, data);
            cmd_list.AddDrawCmd();
        }

        void end_tonemapping()
        {
            auto& cmd_list = *ImGui::GetWindowDrawList();
            cmd_list.AddDrawCmd();
            cmd_list.AddCallback(tonemapping_restore, nullptr);
            cmd_list.AddDrawCmd();
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
        if (render_job_count == 0 && ImGui::Button("Render")) {
            selected_render_image = images.size();
            render_all_views(app::scene_instance());
        }
        ImGui::SameLine();
        ImGui::Text(render_job_count == 0 ? "Standby." : "Rendering...");
        ImGui::Separator();
        if (ImGui::Button("ImGui Demo")) show_test_window ^= 1;
        ImGui::End();

        if (show_test_window) {
            ImGui::SetNextWindowPos(ImVec2(300, 20), ImGuiSetCond_Appearing);
            ImGui::ShowTestWindow(&show_test_window);
        }

        if (images.size()) {
            static float tonemap_min = 0;
            static float tonemap_max = 10;

            ImGui::SetNextWindowPos(ImVec2(250, 20), ImGuiSetCond_Appearing);
            ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiSetCond_Appearing);
            ImGui::Begin("Image Viewer");

            // TODO: should be able to be tinted: separate tint and strength
            ImGui::DragFloat("Blackpoint", &tonemap_min, 0.001, -100, 10000, "%.2f", 10);
            ImGui::DragFloat("Whitepoint", &tonemap_max, 0.001, -100, 10000, "%.2f", 10);
            ImGui::Separator();

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
                begin_tonemapping(tonemap_min, tonemap_max);
                ImGui::Image(&image.get(), ImVec2(image.width(), image.height()));
                end_tonemapping();
            } else {
                ImGui::Text("No image selected");
            }
            ImGui::EndChild();

            ImGui::End();
        }

        upload_render_results();
    }
}

