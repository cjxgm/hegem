#include "../lib/gl/gl.hh"
#include "../lib/imgui.hh"
#include "../util/journal.hh"
#include "../util/constraints.hh"
#include "../glu/traits.hh"
#include <cstddef>
#include <stdexcept>

namespace
{
    namespace imgui_details
    {
        using rt::util::journal;
        journal j() { return {"IMGUI"}; }

        char const* shader_program_source[2] = {
            #include "imgui.shader.inl"
        };

        struct imgui_impl : rt::util::non_transferable
        {
            imgui_impl()
            {
                j() << "(ctor)\n";
                compile_shader();
                upload_font();
                prepare_input();
                setup_style();
            }

            ~imgui_impl()
            {
                j() << "(dtor)\n";
                auto& io = ImGui::GetIO();
                io.Fonts->TexID = nullptr;

                gl::delete_buffers(2, vertex_buffers);
                gl::delete_vertex_arrays(1, &vao);
                gl::delete_textures(1, &tex_font);
                gl::delete_program(program);

                ImGui::Shutdown();
            }

            void on_char(unsigned int codepoint)
            {
                if (codepoint == 0) return;

                auto& io = ImGui::GetIO();
                if (codepoint < 0x1'00'00)
                    io.AddInputCharacter(codepoint);
                else
                    j() << "ImGui is incapable of processing character " << (int)codepoint << "\n";
            }

            void on_key(int key, bool down, unsigned int mods)
            {
                auto& io = ImGui::GetIO();
                io.KeysDown[key] = down;
                io.KeyCtrl  = (1 << 0) & mods;
                io.KeyShift = (1 << 1) & mods;
                io.KeyAlt   = (1 << 2) & mods;
                io.KeySuper = (1 << 3) & mods;
            }

            void on_scroll(float /*x*/, float y)
            {
                mouse_scroll_y += y;
            }

            void on_mouse_button(int button, bool down)
            {
                if (button < 0) throw std::runtime_error{"WTF? You have an odd mouse!"};
                if (button >= 3) return;    // only support 3-button mouse
                if (down) mouse_once_down[button] = true;
            }

            void on_framebuffer_resized(int w, int h, int fbw, int fbh)
            {
                j() << "resized window=" << w << "x" << h << " framebuffer=" << fbw << "x" << fbh << "\n";
                auto& io = ImGui::GetIO();
                io.DisplaySize = ImVec2(w, h);
                io.DisplayFramebufferScale = ImVec2((float)fbw / w, (float)fbh / h);

                j() << "update projection matrix\n";
                float proj[16] = {
                     2.0f/w,  0.0f  ,  0.0f, 0.0f,
                     0.0f  , -2.0f/h,  0.0f, 0.0f,
                     0.0f  ,  0.0f  , -1.0f, 0.0f,
                    -1.0f  ,  1.0f  ,  0.0f, 1.0f,
                };
                gl::program_uniform_matrix4fv(program, 0, 1, false, proj);
            }

            template <class Context>
            void on_frame_begin(Context& ctx)
            {
                auto& io = ImGui::GetIO();

                // frame time
                auto now = ctx.now();
                io.DeltaTime = time == 0
                    ? 1.0f / 60.0f
                    : static_cast<float>(now - time);
                time = now;

                // mouse position
                if (ctx.focused()) {
                    auto pos = ctx.cursor_position();
                    io.MousePos = ImVec2(pos.x, pos.y);
                } else {
                    io.MousePos = ImVec2(-1, -1);
                }

                // mouse button
                for (auto i=0u; i < sizeof(mouse_once_down)/sizeof(*mouse_once_down); i++) {
                    io.MouseDown[i] = mouse_once_down[i] || ctx.mouse_button_is_down(i);
                    mouse_once_down[i] = false;
                }

                // mouse scroll (only support 1 axis)
                io.MouseWheel = mouse_scroll_y;
                mouse_scroll_y = 0;

                ImGui::NewFrame();
            }

            void on_frame_end()
            {
                ImGui::Render();

                auto& draw = *ImGui::GetDrawData();
                auto& io = ImGui::GetIO();
                draw.ScaleClipRects(io.DisplayFramebufferScale);

                int fbw = io.DisplaySize.x * io.DisplayFramebufferScale.x;
                int fbh = io.DisplaySize.y * io.DisplayFramebufferScale.y;
                gl::funcs::viewport(0, 0, fbw, fbh);

                gl::disable(gl::enums::cull_face);
                gl::disable(gl::depth_test);
                gl::enable(gl::scissor_test);
                gl::enable(gl::blend);

                gl::blend_equation(gl::func_add);
                gl::blend_func(gl::src_alpha, gl::one_minus_src_alpha);

                gl::bind_vertex_array(vao);
                gl::use_program(program);

                // TODO: optimize this with Ranges TS
                for (auto first=draw.CmdLists, last=first+draw.CmdListsCount; first < last; first++) {
                    auto& cmd_list = **first;

                    gl::named_buffer_data(
                            vertex_buffers[0],
                            cmd_list.VtxBuffer.Size * sizeof(ImDrawVert),
                            cmd_list.VtxBuffer.Data,
                            gl::stream_draw);
                    gl::named_buffer_data(
                            vertex_buffers[1],
                            cmd_list.IdxBuffer.Size * sizeof(ImDrawIdx),
                            cmd_list.IdxBuffer.Data,
                            gl::stream_draw);

                    ImDrawIdx const* idx_buffer_offset{};
                    for (auto& cmd: cmd_list.CmdBuffer) {
                        if (cmd.UserCallback) {
                            cmd.UserCallback(&cmd_list, &cmd);
                        } else {
                            int x1 = cmd.ClipRect.x;
                            int y1 = cmd.ClipRect.y;
                            int x2 = cmd.ClipRect.z;
                            int y2 = cmd.ClipRect.w;
                            gl::scissor(x1, fbh-y2, x2-x1, y2-y1);
                            gl::bind_texture_unit(0, *static_cast<gl::uint_type*>(cmd.TextureId));
                            gl::draw_elements(
                                    gl::triangles,
                                    cmd.ElemCount,
                                    rt::glu::traits::equiv_unsigned_type_enum<ImDrawIdx>,
                                    idx_buffer_offset);
                        }
                        idx_buffer_offset += cmd.ElemCount;
                    }
                }

                // TODO: use a state tracker instead
                gl::disable(gl::scissor_test);
            }

        private:
            double time{};
            bool mouse_once_down[3]{};
            float mouse_scroll_y{};
            gl::uint_type program{};
            gl::uint_type tex_font{};
            gl::uint_type vao{};
            gl::uint_type vertex_buffers[2]{};  // { vertices, elements }

            void compile_shader()
            {
                j() << "compiling shader\n";

                auto vert = gl::create_shader(gl::vertex_shader);
                gl::shader_source(vert, 1, &shader_program_source[0], nullptr);
                gl::compile_shader(vert);

                auto frag = gl::create_shader(gl::fragment_shader);
                gl::shader_source(frag, 1, &shader_program_source[1], nullptr);
                gl::compile_shader(frag);

                program = gl::create_program();
                gl::attach_shader(program, vert);
                gl::attach_shader(program, frag);
                gl::link_program(program);

                gl::delete_shader(frag);
                gl::delete_shader(vert);

                gl::program_uniform1i(program, 1, 0);      // bind "tex" to unit 0
            }

            void upload_font()
            {
                auto& io = ImGui::GetIO();

                j() << "generating font texture\n";
                gl::ubyte_type* pixels;
                int w, h;
                ImFontConfig config;
                config.OversampleH = 2;
                config.OversampleV = 2;
                config.GlyphExtraSpacing.x = 0.0f;
                config.PixelSnapH = true;
                io.Fonts->AddFontFromFileTTF("support/fonts/ui.ttf", 16, &config);
                io.Fonts->GetTexDataAsAlpha8(&pixels, &w, &h);

                j() << "uploading font texture\n";
                gl::create_textures(gl::texture_2d, 1, &tex_font);
                gl::texture_storage2d(tex_font, 1, gl::r8, w, h);
                gl::texture_sub_image2d(
                        tex_font,
                        0,                              // LOD
                        0, 0, w, h,                     // geometry
                        gl::red, gl::unsigned_byte,     // format
                        pixels);
                gl::texture_parameteri(tex_font, gl::texture_min_filter, gl::linear);
                gl::texture_parameteri(tex_font, gl::texture_mag_filter, gl::linear);

                io.Fonts->TexID = &tex_font;
            }

            void prepare_input()
            {
                j() << "creating vertex array and buffers\n";
                gl::create_vertex_arrays(1, &vao);
                gl::create_buffers(2, vertex_buffers);

                gl::enable_vertex_array_attrib(vao, 0);     // vec2 pos
                gl::enable_vertex_array_attrib(vao, 1);     // vec2 uv
                gl::enable_vertex_array_attrib(vao, 2);     // vec4 color

                gl::vertex_array_attrib_format(
                        vao, 0,
                        2, gl::float_, false,
                        offsetof(ImDrawVert, pos));
                gl::vertex_array_attrib_format(
                        vao, 1,
                        2, gl::float_, false,
                        offsetof(ImDrawVert, uv));
                gl::vertex_array_attrib_format(
                        vao, 2,
                        4, gl::unsigned_byte, true,
                        offsetof(ImDrawVert, col));

                gl::vertex_array_vertex_buffer(vao, 0, vertex_buffers[0], 0, sizeof(ImDrawVert));
                gl::vertex_array_element_buffer(vao, vertex_buffers[1]);

                gl::vertex_array_attrib_binding(vao, 0, 0);
                gl::vertex_array_attrib_binding(vao, 1, 0);
                gl::vertex_array_attrib_binding(vao, 2, 0);
            }

            void setup_style()
            {
                auto& style = ImGui::GetStyle();
                style.WindowPadding       = ImVec2(10, 10);
                style.WindowRounding      = 2;
                style.ChildWindowRounding = 2;
                style.FramePadding        = ImVec2(4, 4);
                style.FrameRounding       = 1;
                style.ItemSpacing         = ImVec2(8, 4);
                style.IndentSpacing       = 22;
                style.ScrollbarSize       = 16;
                style.ScrollbarRounding   = 2;
                style.GrabRounding        = 1;
            }
        };
    }

    using imgui_details::imgui_impl;
}

