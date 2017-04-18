#include "../lib/gl/gl.hh"
#include "../lib/imgui.hh"
#include "hdr-texture.hh"

namespace rt::gui
{
    namespace
    {
        void enable_hdr(ImDrawList const* /*pdraw_list*/, ImDrawCmd const* pcmd)
        {
            auto hdr = static_cast<hdr_texture*>(pcmd->UserCallbackData);
            gl::uniform1i(2, 1);                        // HDR mode
            gl::uniform3fv(3, 1, &hdr->blackpoint[0]);  // blackpoint
            gl::uniform3fv(4, 1, &hdr->whitepoint[0]);  // whitepoint
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
        ImGui::Image(hdr->tex.get().ptr(), ImVec2(hdr->w, hdr->h));
        cmd_list.AddDrawCmd();
        cmd_list.AddCallback(disable_hdr, nullptr);
        cmd_list.AddDrawCmd();
    }
}

