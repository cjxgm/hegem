#include "../lib/glm/vec3.hh"
#include "../lib/glm/vec4.hh"
#include "../lib/imgui.hh"
#include "../glu/cast.hh"
#include "image-viewer.hh"
#include <utility>      // for std::move

namespace rt::morpha
{
    namespace
    {
        void general_mode(ImDrawList const* /*pdraw_list*/, ImDrawCmd const* /*pcmd*/)
        {
            gl::uniform1i(2, 0);                        // 0 = general mode
        }

        void ldr_mode(ImDrawList const* /*pdraw_list*/, ImDrawCmd const* /*pcmd*/)
        {
            gl::uniform1i(2, 2);                        // 2 = LDR mode
        }
    }

    struct image_viewer::temporary_state
    {
        int w{};
        int h{};
        glu::shared_texture2d tex{};
    };

    image_viewer::image_viewer(): tmp{std::make_unique<temporary_state>()} {}
    image_viewer::~image_viewer() = default;

    auto image_viewer::drawable() const -> bool
    {
        return (tmp->tex != 0);
    }

    void image_viewer::draw(float scale)
    {
        if (!drawable()) return;

        auto& cmd_list = *ImGui::GetWindowDrawList();
        cmd_list.AddDrawCmd();
        cmd_list.AddCallback(ldr_mode, nullptr);
        ImGui::Image(glu::cast::id_to_ptr(tmp->tex), ImVec2(float(tmp->w) * scale, float(tmp->h) * scale));
        cmd_list.AddDrawCmd();
        cmd_list.AddCallback(general_mode, nullptr);
    }

    void image_viewer::clear(image::color::linear_rgb color)
    {
        if (!drawable()) return;

        auto rgba = glm::vec4{color, 1.0f};
        gl::clear_tex_image(tmp->tex, 0, gl::rgba, gl::float_, &rgba[0]);
    }

    void image_viewer::resize(int w, int h)
    {
        if (w > 0 && h > 0) {
            auto& tex = tmp->tex;
            tex = glu::texture2d_pool::instance().allocate();
            gl::texture_storage2d(tex, 1, gl::rgba32f, w, h);
            gl::texture_parameteri(tex, gl::texture_min_filter, gl::linear);
            gl::texture_parameteri(tex, gl::texture_mag_filter, gl::linear);
        } else {
            tmp->tex = {};
        }

        tmp->w = w;
        tmp->h = h;
    }

    auto image_viewer::texture() const -> glu::shared_texture2d
    {
        return tmp->tex;
    }

    auto image_viewer::width() const -> int
    {
        return tmp->w;
    }

    auto image_viewer::height() const -> int
    {
        return tmp->h;
    }
}

