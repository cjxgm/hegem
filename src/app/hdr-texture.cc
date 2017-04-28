#include "../lib/gl/glfw.hh"
#include "../lib/gl/gl.hh"
#include "../lib/glm/op/common.hh"
#include "../lib/glm/op/geom.hh"
#include "../lib/imgui.hh"
#include "../glu/cast.hh"
#include "../glu/shader.hh"
#include "../glu/states.hh"
#include "hdr-texture.hh"

namespace rt::app
{
    namespace
    {
        static constexpr auto eps = 1e-3f;
        static_assert(eps < 1.0f / 255.0f, "Epsilon is not small enough.");
        static_assert(eps > 0.1f / 255.0f, "Epsilon is too small.");

        float const black[4]{0.0f, 0.0f, 0.0f, 1.0f};
        float const transparent[4]{0.0f, 0.0f, 0.0f, 0.0f};

        void general_mode(ImDrawList const* /*pdraw_list*/, ImDrawCmd const* /*pcmd*/)
        {
            gl::uniform1i(2, 0);                        // general mode
        }

        void hdr_mode(ImDrawList const* /*pdraw_list*/, ImDrawCmd const* pcmd)
        {
            auto hdr = static_cast<hdr_texture*>(pcmd->UserCallbackData);
            gl::uniform1i(2, 1);                        // HDR mode
            gl::uniform3fv(3, 1, &hdr->blackpoint[0]);  // blackpoint
            gl::uniform3fv(4, 1, &hdr->whitepoint[0]);  // whitepoint
            gl::uniform1f(5, glfwGetTime());            // time
            gl::uniform1f(6, hdr->dither_amount);       // dither amount
        }

        void ldr_mode(ImDrawList const* /*pdraw_list*/, ImDrawCmd const* pcmd)
        {
            gl::uniform1i(2, 2);                        // LDR mode
        }
    }

    void imgui_hdr_texture(hdr_texture* hdr)
    {
        auto& cmd_list = *ImGui::GetWindowDrawList();
        cmd_list.AddDrawCmd();
        cmd_list.AddCallback(hdr_mode, hdr);
        auto pos = ImGui::GetCursorPos();
        ImGui::Image(glu::cast::id_to_ptr(hdr->tex), ImVec2(hdr->w, hdr->h));
        cmd_list.AddDrawCmd();
        cmd_list.AddCallback(ldr_mode, nullptr);
        ImGui::SetCursorPos(pos);
        ImGui::Image(glu::cast::id_to_ptr(hdr->markers), ImVec2(hdr->w, hdr->h));
        cmd_list.AddDrawCmd();
        cmd_list.AddCallback(general_mode, nullptr);
    }

    void imgui_hdr_color(
            char const* color_label,
            char const* intensity_label,
            glm::vec3* color,
            float intensity_speed,
            float intensity_min,
            float intensity_max,
            char const* intensity_format,
            float intensity_power)
    {
        auto base = abs(*color);
        auto len = glm::max(base.x, glm::max(base.y, base.z));
        bool too_small = (len < eps);
        if (too_small) {
            base = glm::vec3{0};
        } else {
            if (color->x < 0) len = -len;
            base = *color / len;
        }

        ImGui::PushID(color_label);
        ImGui::ColorEdit3(color_label, &base[0]);
        ImGui::DragFloat(
                intensity_label,
                &len,
                intensity_speed,
                intensity_min,
                intensity_max,
                intensity_format,
                intensity_power);
        ImGui::PopID();

        *color = too_small ? glm::vec3{len} : base * len;
    }

    hdr_texture::hdr_texture(int w, int h)
        : w{w}
        , h{h}
        , tex{glu::texture2d_pool::instance().allocate()}
        , markers{glu::texture2d_pool::instance().allocate()}
    {
        gl::texture_storage2d(tex, 1, gl::rgba32f, w, h);
        gl::clear_tex_image(tex, 0, gl::rgba, gl::float_, black);
        gl::texture_parameteri(tex, gl::texture_min_filter, gl::linear);
        gl::texture_parameteri(tex, gl::texture_mag_filter, gl::nearest);

        gl::texture_storage2d(markers, 1, gl::rgba32f, w, h);
        gl::clear_tex_image(markers, 0, gl::rgba, gl::float_, transparent);
        gl::texture_parameteri(markers, gl::texture_min_filter, gl::linear);
        gl::texture_parameteri(markers, gl::texture_mag_filter, gl::nearest);

        prog = glu::shader_factory::program_from_name("working-tile-marker");
        vao = glu::vertex_array_pool::instance().allocate();
        fbo = glu::framebuffer_pool::instance().allocate();
        gl::named_framebuffer_draw_buffer(fbo, gl::color_attachment0);
        gl::named_framebuffer_texture(fbo, gl::color_attachment0, markers, 0);
    }

    void hdr_texture::mark(util::tile tile)
    {
        gl::funcs::viewport(tile.x, tile.y, tile.w, tile.h);
        glu::states_manager::instance().enable_only({});

        gl::bind_framebuffer(gl::framebuffer, fbo);
        gl::bind_vertex_array(vao);
        gl::use_program(prog);
        gl::draw_arrays(gl::points, 0, 1);
    }

    void hdr_texture::unmark(util::tile tile)
    {
        gl::clear_tex_sub_image(
            markers,
            0,
            tile.x, tile.y, 0, tile.w, tile.h, 1,
            gl::rgba, gl::float_,
            transparent);
    }
}

