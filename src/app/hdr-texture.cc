#include "../lib/gl/glfw.hh"
#include "../lib/gl/gl.hh"
#include "../lib/glm/op/common.hh"
#include "../lib/glm/op/geom.hh"
#include "../lib/imgui.hh"
#include "../glu/cast.hh"
#include "hdr-texture.hh"

namespace rt::app
{
    namespace
    {
        static constexpr auto eps = 1e-3f;
        static_assert(eps < 1.0f / 255.0f, "Epsilon is not small enough.");
        static_assert(eps > 0.1f / 255.0f, "Epsilon is too small.");

        void enable_hdr(ImDrawList const* /*pdraw_list*/, ImDrawCmd const* pcmd)
        {
            auto hdr = static_cast<hdr_texture*>(pcmd->UserCallbackData);
            gl::uniform1i(2, 1);                        // HDR mode
            gl::uniform3fv(3, 1, &hdr->blackpoint[0]);  // blackpoint
            gl::uniform3fv(4, 1, &hdr->whitepoint[0]);  // whitepoint
            gl::uniform1f(5, glfwGetTime());            // time
            gl::uniform1f(6, hdr->dither_amount);       // dither amount
        }

        void disable_hdr(ImDrawList const* /*pdraw_list*/, ImDrawCmd const* /*pcmd*/)
        {
            gl::uniform1i(2, 0);                        // general mode
        }
    }

    void imgui_hdr_texture(hdr_texture* hdr)
    {
        auto& cmd_list = *ImGui::GetWindowDrawList();
        cmd_list.AddDrawCmd();
        cmd_list.AddCallback(enable_hdr, hdr);
        cmd_list.AddDrawCmd();
        ImGui::Image(glu::cast::id_to_ptr(hdr->tex), ImVec2(hdr->w, hdr->h));
        cmd_list.AddDrawCmd();
        cmd_list.AddCallback(disable_hdr, nullptr);
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
}

