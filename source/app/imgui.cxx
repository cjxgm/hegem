#include "../lib/gl/gl.hxx"
#include "../lib/gl/glfw.hxx"
#include "../lib/imgui.hxx"
#include "../tool/journal.hxx"
#include "../tool/constraints.hxx"
#include "../glu/resource.hxx"
#include "../glu/traits.hxx"
#include "../glu/states.hxx"
#include "../glu/cast.hxx"
#include "../glu/shader.hxx"
#include "imgui.hxx"
#include <cstddef>
#include <stdexcept>
#include <memory>
#include <vector>

namespace hegem::app::imgui
{
    namespace
    {
        using hegem::tool::journal;
        journal j() { return {"IMGUI"}; }

        struct glfw_cursor_deleter
        {
            auto operator () (GLFWcursor* cursor) -> void
            {
                if (cursor) glfwDestroyCursor(cursor);
            }
        };

        struct context: tool::non_transferable
        {
            using unique_cursor = std::unique_ptr<GLFWcursor, glfw_cursor_deleter>;

            double time{};
            bool mouse_once_down[3]{};
            float mouse_scroll_y{};
            glu::shared_program program;
            glu::shared_texture2d tex_font;
            glu::shared_vertex_array vao;
            glu::shared_buffer vertices_buffer;
            glu::shared_buffer elements_buffer;
            std::vector<unique_cursor> cursors;

            context()
            {
                j() << "(ctor)\n";
                ImGui::CreateContext();
                compile_shader();
                upload_font();
                prepare_input();
                setup_style();
                setup_keymap();
                setup_cursors();
                disable_file_writing();
            }

            ~context()
            {
                j() << "(dtor)\n";
                auto& io = ImGui::GetIO();
                io.Fonts->TexID = nullptr;
                ImGui::DestroyContext();
            }

            static context& instance()
            {
                static context ctx;
                return ctx;
            }

        private:
            auto compile_shader() -> void
            {
                j() << "compiling shader\n";
                program = glu::shader_factory::program_from_name("imgui");
                gl::program_uniform1i(program, 1, 0);      // bind "tex" to unit 0
            }

            auto upload_font() -> void
            {
                auto& io = ImGui::GetIO();

                j() << "generating font texture\n";
                gl::ubyte_type* pixels;
                int w, h;
                ImFontConfig config;
                config.OversampleH = 4;
                config.OversampleV = 4;
                config.GlyphExtraSpacing.x = 0.0f;
                config.PixelSnapH = true;
                io.Fonts->AddFontFromFileTTF("support/fonts/ui.ttf", 16, &config);
                io.Fonts->GetTexDataAsAlpha8(&pixels, &w, &h);

                j() << "uploading font texture\n";
                tex_font = glu::texture2d_pool::instance().allocate();
                gl::texture_storage2d(tex_font, 1, gl::r8, w, h);
                gl::texture_sub_image2d(
                        tex_font,
                        0,                              // LOD
                        0, 0, w, h,                     // geometry
                        gl::red, gl::unsigned_byte,     // format
                        pixels);
                gl::texture_parameteri(tex_font, gl::texture_min_filter, gl::linear_mipmap_linear);
                gl::texture_parameteri(tex_font, gl::texture_mag_filter, gl::linear);
                gl::generate_texture_mipmap(tex_font);

                io.Fonts->TexID = glu::cast::id_to_ptr(tex_font);
            }

            auto prepare_input() -> void
            {
                j() << "creating vertex array and buffers\n";
                auto& vao_pool = glu::vertex_array_pool::instance();
                auto& buf_pool = glu::buffer_pool::instance();
                vao = vao_pool.allocate();
                vertices_buffer = buf_pool.allocate();
                elements_buffer = buf_pool.allocate();

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

                gl::vertex_array_vertex_buffer(vao, 0, vertices_buffer, 0, sizeof(ImDrawVert));
                gl::vertex_array_element_buffer(vao, elements_buffer);

                gl::vertex_array_attrib_binding(vao, 0, 0);
                gl::vertex_array_attrib_binding(vao, 1, 0);
                gl::vertex_array_attrib_binding(vao, 2, 0);
            }

            auto setup_style() -> void
            {
                ImGui::StyleColorsDark();
                auto& style = ImGui::GetStyle();
                style.WindowPadding       = ImVec2(10, 10);
                style.WindowRounding      = 2;
                style.ChildRounding       = 2;
                style.FramePadding        = ImVec2(4, 4);
                style.FrameRounding       = 1;
                style.ItemSpacing         = ImVec2(8, 4);
                style.IndentSpacing       = 22;
                style.ScrollbarSize       = 16;
                style.ScrollbarRounding   = 2;
                style.GrabRounding        = 1;
            }

            auto setup_keymap() -> void
            {
                auto& io = ImGui::GetIO();

                // Special keys
                io.KeyMap[ImGuiKey_Tab]        = GLFW_KEY_TAB;
                io.KeyMap[ImGuiKey_LeftArrow]  = GLFW_KEY_LEFT;
                io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
                io.KeyMap[ImGuiKey_UpArrow]    = GLFW_KEY_UP;
                io.KeyMap[ImGuiKey_DownArrow]  = GLFW_KEY_DOWN;
                io.KeyMap[ImGuiKey_PageUp]     = GLFW_KEY_PAGE_UP;
                io.KeyMap[ImGuiKey_PageDown]   = GLFW_KEY_PAGE_DOWN;
                io.KeyMap[ImGuiKey_Home]       = GLFW_KEY_HOME;
                io.KeyMap[ImGuiKey_End]        = GLFW_KEY_END;
                io.KeyMap[ImGuiKey_Delete]     = GLFW_KEY_DELETE;
                io.KeyMap[ImGuiKey_Backspace]  = GLFW_KEY_BACKSPACE;
                io.KeyMap[ImGuiKey_Enter]      = GLFW_KEY_ENTER;
                io.KeyMap[ImGuiKey_Escape]     = GLFW_KEY_ESCAPE;

                // Only these letter keys are used for shortcut purpose
                io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
                io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
                io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
                io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
                io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
                io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
            }

            auto setup_cursors() -> void
            {
                cursors.resize(int(ImGuiMouseCursor_COUNT));
                cursors[int(ImGuiMouseCursor_Arrow)].reset(glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
                cursors[int(ImGuiMouseCursor_TextInput)].reset(glfwCreateStandardCursor(GLFW_IBEAM_CURSOR));
                cursors[int(ImGuiMouseCursor_ResizeAll)].reset(glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR));
                cursors[int(ImGuiMouseCursor_ResizeNS)].reset(glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR));
                cursors[int(ImGuiMouseCursor_ResizeEW)].reset(glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR));
                cursors[int(ImGuiMouseCursor_ResizeNESW)].reset(glfwCreateStandardCursor(GLFW_HAND_CURSOR));
                cursors[int(ImGuiMouseCursor_ResizeNWSE)].reset(glfwCreateStandardCursor(GLFW_HAND_CURSOR));
            }

            auto disable_file_writing() -> void
            {
                auto& io = ImGui::GetIO();
                io.IniFilename = nullptr;
                io.LogFilename = nullptr;
            }
        };
    }

    auto init_once() -> void
    {
        context::instance();
    }

    auto on_char(GLFWwindow* win, unsigned int codepoint) -> void
    {
        if (codepoint == 0) return;

        auto& io = ImGui::GetIO();
        if (codepoint < 0x1'00'00) {
            io.AddInputCharacter(codepoint);
        } else {
            j() << "ImGui is incapable of processing character " << (int)codepoint << "\n";
        }
    }

    auto on_key(GLFWwindow* win, int key, bool down) -> void
    {
        auto& io = ImGui::GetIO();
        io.KeysDown[key] = down;

        io.KeyCtrl =
            io.KeysDown[GLFW_KEY_LEFT_CONTROL] ||
            io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift =
            io.KeysDown[GLFW_KEY_LEFT_SHIFT] ||
            io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt =
            io.KeysDown[GLFW_KEY_LEFT_ALT] ||
            io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper =
            io.KeysDown[GLFW_KEY_LEFT_SUPER] ||
            io.KeysDown[GLFW_KEY_RIGHT_SUPER];
    }

    auto on_scroll(GLFWwindow* win, float x, float y) -> void
    {
        auto& ctx = context::instance();
        ctx.mouse_scroll_y += y;
    }

    auto on_mouse_button(GLFWwindow* win, int button, bool down) -> void
    {
        auto& ctx = context::instance();
        if (button < 0) throw std::runtime_error{"WTF? You have an odd mouse!"};
        if (button >= 3) return;    // only support 3-button mouse
        if (down) ctx.mouse_once_down[button] = true;
    }

    auto on_framebuffer_resized(GLFWwindow* win, int w, int h, int fbw, int fbh) -> void
    {
        auto& ctx = context::instance();

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
        gl::program_uniform_matrix4fv(ctx.program, 0, 1, false, proj);
    }

    auto on_frame_begin(GLFWwindow* win) -> void
    {
        auto& ctx = context::instance();
        auto& io = ImGui::GetIO();

        // frame time
        auto now = glfwGetTime();
        io.DeltaTime = ctx.time == 0
            ? 1.0f / 60.0f
            : static_cast<float>(now - ctx.time);
        ctx.time = now;

        // mouse position
        if (glfwGetWindowAttrib(win, GLFW_FOCUSED)) {
            double x, y;
            glfwGetCursorPos(win, &x, &y);
            io.MousePos = ImVec2(x, y);
        } else {
            io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
        }

        // mouse button
        for (auto i=0u; i < sizeof(ctx.mouse_once_down)/sizeof(*ctx.mouse_once_down); i++) {
            io.MouseDown[i] = ctx.mouse_once_down[i] || glfwGetMouseButton(win, i);
            ctx.mouse_once_down[i] = false;
        }

        // mouse scroll (only support 1 axis)
        io.MouseWheel = ctx.mouse_scroll_y;
        ctx.mouse_scroll_y = 0;

        auto cursor = ImGui::GetMouseCursor();
        if (cursor == ImGuiMouseCursor_None) cursor = ImGuiMouseCursor_Arrow;
        glfwSetCursor(win, ctx.cursors[int(cursor)].get());

        ImGui::NewFrame();
    }

    auto on_frame_end(GLFWwindow* win) -> void
    {
        ImGui::Render();

        auto& ctx = context::instance();
        auto& draw = *ImGui::GetDrawData();
        auto& io = ImGui::GetIO();
        draw.ScaleClipRects(io.DisplayFramebufferScale);

        int fbw = io.DisplaySize.x * io.DisplayFramebufferScale.x;
        int fbh = io.DisplaySize.y * io.DisplayFramebufferScale.y;
        gl::funcs::viewport(0, 0, fbw, fbh);

        glu::states_manager::instance().enable_only({
            gl::scissor_test,
            gl::blend,
        });

        gl::blend_equation(gl::func_add);
        gl::blend_func(gl::src_alpha, gl::one_minus_src_alpha);

        gl::bind_framebuffer(gl::framebuffer, 0);
        gl::bind_vertex_array(ctx.vao);
        gl::use_program(ctx.program);

        // TODO: optimize this with Ranges TS
        for (auto first=draw.CmdLists, last=first+draw.CmdListsCount; first < last; first++) {
            auto& cmd_list = **first;

            gl::named_buffer_data(
                    ctx.vertices_buffer,
                    cmd_list.VtxBuffer.Size * sizeof(ImDrawVert),
                    cmd_list.VtxBuffer.Data,
                    gl::stream_draw);
            gl::named_buffer_data(
                    ctx.elements_buffer,
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
                    auto tex = glu::cast::ptr_to_id(cmd.TextureId);
                    gl::bind_texture_unit(0, tex);
                    if (cmd.ElemCount != 0) {
                        gl::scissor(x1, fbh-y2, x2-x1, y2-y1);
                        gl::draw_elements(
                                gl::triangles,
                                cmd.ElemCount,
                                glu::traits::equiv_unsigned_type_enum<ImDrawIdx>,
                                idx_buffer_offset);
                    }
                }
                idx_buffer_offset += cmd.ElemCount;
            }
        }
    }
}

