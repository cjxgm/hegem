#include "../lib/gl/glfw.hxx"
#include "../lib/gl/gl.hxx"
#include "../lib/glm/op/common.hxx"
#include "../lib/glm/op/geom.hxx"
#include "../lib/imgui.hxx"
#include "../glu/cast.hxx"
#include "../glu/shader.hxx"
#include "../glu/states.hxx"
#include "hdr-texture.hxx"

namespace hegem::app
{
    inline namespace
    {
        static constexpr auto eps = 1e-3f;
        static_assert(eps < 1.0f / 255.0f, "Epsilon is not small enough.");
        static_assert(eps > 0.1f / 255.0f, "Epsilon is too small.");

        float const black[4]{0.0f, 0.0f, 0.0f, 1.0f};
        float const transparent[4]{0.0f, 0.0f, 0.0f, 0.0f};

        auto general_mode(ImDrawList const* pdraw_list, ImDrawCmd const* pcmd) -> void
        {
            gl::uniform1i(2, 0);                        // general mode
        }

        auto hdr_mode(ImDrawList const* pdraw_list, ImDrawCmd const* pcmd) -> void
        {
            auto hdr = static_cast<hdr_texture*>(pcmd->UserCallbackData);
            gl::uniform1i(2, 1);                        // HDR mode
            gl::uniform3fv(3, 1, &hdr->blackpoint[0]);  // blackpoint
            gl::uniform3fv(4, 1, &hdr->whitepoint[0]);  // whitepoint
            gl::uniform1f(5, glfwGetTime());            // time
            gl::uniform1f(6, hdr->dither_amount);       // dither amount
        }

        auto ldr_mode(ImDrawList const* pdraw_list, ImDrawCmd const* pcmd) -> void
        {
            gl::uniform1i(2, 2);                        // LDR mode
        }

        struct context
        {
            glu::shared_program prog;
            glu::shared_vertex_array vao;

            static context& instance()
            {
                static context ctx;
                return ctx;
            }

        private:
            context()
            {
                prog = glu::shader_factory::program_from_name("working-tile-marker");
                vao = glu::vertex_array_pool::instance().allocate();
            }
        };
    }

    auto imgui_hdr_texture(hdr_texture* hdr, char const* drag_receiver) -> void
    {
        auto& io = ImGui::GetIO();
        auto& cmd_list = *ImGui::GetWindowDrawList();
        cmd_list.AddDrawCmd();
        cmd_list.AddCallback(hdr_mode, hdr);
        auto pos = ImGui::GetCursorPos();
        auto spos = ImGui::GetCursorScreenPos();
        ImGui::Image(glu::cast::id_to_ptr(hdr->tex), ImVec2(hdr->w, hdr->h));
        cmd_list.AddDrawCmd();
        cmd_list.AddCallback(ldr_mode, nullptr);
        ImGui::SetCursorPos(pos);
        ImGui::Image(glu::cast::id_to_ptr(hdr->markers), ImVec2(hdr->w, hdr->h));
        cmd_list.AddDrawCmd();
        cmd_list.AddCallback(general_mode, nullptr);

        // mouse dragging handling
        ImGui::SetCursorPos(pos);
        ImGui::InvisibleButton(drag_receiver, ImVec2(hdr->w, hdr->h));
        if (ImGui::IsItemActive()) {
            if (ImGui::IsMouseDoubleClicked(0)) {
                hdr->double_clicked = true;
                auto mpos = ImGui::GetMousePos();
                hdr->image_local_clicked_pos = { mpos.x - spos.x, mpos.y - spos.y };
            } else {
                auto offset = io.MouseDelta;
                hdr->dragging = true;
                hdr->drag_offset = { offset.x, offset.y };
            }
        } else {
            hdr->dragging = false;
            hdr->drag_offset = {};

            hdr->double_clicked = false;
            hdr->image_local_clicked_pos = {};
        }
    }

    auto imgui_hdr_color(
        char const* color_label,
        char const* intensity_label,
        glm::vec3* color,
        float intensity_speed,
        float intensity_min,
        float intensity_max,
        char const* intensity_format,
        float intensity_power
    ) -> void
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

        fbo = glu::framebuffer_pool::instance().allocate();
        gl::named_framebuffer_draw_buffer(fbo, gl::color_attachment0);
        gl::named_framebuffer_texture(fbo, gl::color_attachment0, markers, 0);
    }

    auto hdr_texture::mark(tool::tile tile) -> void
    {
        auto& ctx = context::instance();
        gl::funcs::viewport(tile.x, tile.y, tile.w, tile.h);
        glu::states_manager::instance().enable_only({});

        gl::bind_framebuffer(gl::framebuffer, fbo);
        gl::bind_vertex_array(ctx.vao);
        gl::use_program(ctx.prog);
        gl::draw_arrays(gl::points, 0, 1);
    }

    auto hdr_texture::unmark(tool::tile tile) -> void
    {
        gl::clear_tex_sub_image(
            markers,
            0,
            tile.x, tile.y, 0, tile.w, tile.h, 1,
            gl::rgba, gl::float_,
            transparent);
    }
}

