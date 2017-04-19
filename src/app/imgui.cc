#include "../lib/gl/glfw.hh"
#include "../lib/imgui.hh"
#include "../util/journal.hh"
#include "../glu/resource.hh"
#include "../glu/traits.hh"
#include "imgui.hh"
#include <cstddef>
#include <stdexcept>

namespace rt::app::imgui
{
    namespace
    {
        using rt::util::journal;
        journal j() { return {"IMGUI"}; }

        char const* shader_sources[2] = {
            #include "imgui.shader.inl"
        };

        double time{};
        bool mouse_once_down[3]{};
        float mouse_scroll_y{};
        glu::program program{};
        glu::texture tex_font{};
        glu::vertex_array vao{};
        glu::buffer vertex_buffers[2]{};  // { vertices, elements }

        struct context
        {
            context()
            {
                j() << "(ctor)\n";
                compile_shader();
                upload_font();
                prepare_input();
                setup_style();
                setup_keymap();
            }

            ~context()
            {
                j() << "(dtor)\n";
                auto& io = ImGui::GetIO();
                io.Fonts->TexID = nullptr;
                ImGui::Shutdown();
            }

        private:
            static void compile_shader()
            {
                j() << "compiling shader\n";

                auto vert = gl::create_shader(gl::vertex_shader);
                gl::shader_source(vert, 1, &shader_sources[0], nullptr);
                gl::compile_shader(vert);

                auto frag = gl::create_shader(gl::fragment_shader);
                gl::shader_source(frag, 1, &shader_sources[1], nullptr);
                gl::compile_shader(frag);

                program.reset(gl::create_program());
                gl::attach_shader(program.get(), vert);
                gl::attach_shader(program.get(), frag);
                gl::link_program(program.get());

                gl::delete_shader(frag);
                gl::delete_shader(vert);

                gl::program_uniform1i(program.get(), 1, 0);      // bind "tex" to unit 0
            }

            static void upload_font()
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
                gl::uint_type tex;
                gl::create_textures(gl::texture_2d, 1, &tex);
                tex_font.reset(tex);
                gl::texture_storage2d(tex, 1, gl::r8, w, h);
                gl::texture_sub_image2d(
                        tex,
                        0,                              // LOD
                        0, 0, w, h,                     // geometry
                        gl::red, gl::unsigned_byte,     // format
                        pixels);
                gl::texture_parameteri(tex, gl::texture_min_filter, gl::linear);
                gl::texture_parameteri(tex, gl::texture_mag_filter, gl::linear);

                io.Fonts->TexID = tex_font.get().ptr();
            }

            static void prepare_input()
            {
                j() << "creating vertex array and buffers\n";
                {
                    gl::uint_type vao_id;
                    gl::uint_type buffers[2];
                    gl::create_vertex_arrays(1, &vao_id);
                    gl::create_buffers(2, buffers);
                    vao.reset(vao_id);
                    vertex_buffers[0].reset(buffers[0]);
                    vertex_buffers[1].reset(buffers[1]);
                }

                gl::enable_vertex_array_attrib(vao.get(), 0);     // vec2 pos
                gl::enable_vertex_array_attrib(vao.get(), 1);     // vec2 uv
                gl::enable_vertex_array_attrib(vao.get(), 2);     // vec4 color

                gl::vertex_array_attrib_format(
                        vao.get(), 0,
                        2, gl::float_, false,
                        offsetof(ImDrawVert, pos));
                gl::vertex_array_attrib_format(
                        vao.get(), 1,
                        2, gl::float_, false,
                        offsetof(ImDrawVert, uv));
                gl::vertex_array_attrib_format(
                        vao.get(), 2,
                        4, gl::unsigned_byte, true,
                        offsetof(ImDrawVert, col));

                gl::vertex_array_vertex_buffer(vao.get(), 0, vertex_buffers[0].get(), 0, sizeof(ImDrawVert));
                gl::vertex_array_element_buffer(vao.get(), vertex_buffers[1].get());

                gl::vertex_array_attrib_binding(vao.get(), 0, 0);
                gl::vertex_array_attrib_binding(vao.get(), 1, 0);
                gl::vertex_array_attrib_binding(vao.get(), 2, 0);
            }

            static void setup_style()
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

            static void setup_keymap()
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
        };
    }

    void init_once()
    {
        static context _{};
    }

    void on_char(GLFWwindow* win, unsigned int codepoint)
    {
        (void)win;

        if (codepoint == 0) return;

        auto& io = ImGui::GetIO();
        if (codepoint < 0x1'00'00) {
            io.AddInputCharacter(codepoint);
        } else {
            j() << "ImGui is incapable of processing character " << (int)codepoint << "\n";
        }
    }

    void on_key(GLFWwindow* win, int key, bool down)
    {
        (void)win;

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

    void on_scroll(GLFWwindow* win, float x, float y)
    {
        (void)win;
        (void)x;

        mouse_scroll_y += y;
    }

    void on_mouse_button(GLFWwindow* win, int button, bool down)
    {
        (void)win;

        if (button < 0) throw std::runtime_error{"WTF? You have an odd mouse!"};
        if (button >= 3) return;    // only support 3-button mouse
        if (down) mouse_once_down[button] = true;
    }

    void on_framebuffer_resized(GLFWwindow* win, int w, int h, int fbw, int fbh)
    {
        (void)win;

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
        gl::program_uniform_matrix4fv(program.get(), 0, 1, false, proj);
    }

    void on_frame_begin(GLFWwindow* win)
    {
        auto& io = ImGui::GetIO();

        // frame time
        auto now = glfwGetTime();
        io.DeltaTime = time == 0
            ? 1.0f / 60.0f
            : static_cast<float>(now - time);
        time = now;

        // mouse position
        if (glfwGetWindowAttrib(win, GLFW_FOCUSED)) {
            double x, y;
            glfwGetCursorPos(win, &x, &y);
            io.MousePos = ImVec2(x, y);
        } else {
            io.MousePos = ImVec2(-1, -1);
        }

        // mouse button
        for (auto i=0u; i < sizeof(mouse_once_down)/sizeof(*mouse_once_down); i++) {
            io.MouseDown[i] = mouse_once_down[i] || glfwGetMouseButton(win, i);
            mouse_once_down[i] = false;
        }

        // mouse scroll (only support 1 axis)
        io.MouseWheel = mouse_scroll_y;
        mouse_scroll_y = 0;

        ImGui::NewFrame();
    }

    void on_frame_end(GLFWwindow* win)
    {
        (void)win;

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

        gl::bind_vertex_array(vao.get());
        gl::use_program(program.get());

        // TODO: optimize this with Ranges TS
        for (auto first=draw.CmdLists, last=first+draw.CmdListsCount; first < last; first++) {
            auto& cmd_list = **first;

            gl::named_buffer_data(
                    vertex_buffers[0].get(),
                    cmd_list.VtxBuffer.Size * sizeof(ImDrawVert),
                    cmd_list.VtxBuffer.Data,
                    gl::stream_draw);
            gl::named_buffer_data(
                    vertex_buffers[1].get(),
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
                    auto tex = static_cast<gl::uint_type>(reinterpret_cast<std::uintptr_t>(cmd.TextureId));
                    gl::scissor(x1, fbh-y2, x2-x1, y2-y1);
                    gl::bind_texture_unit(0, tex);
                    gl::draw_elements(
                            gl::triangles,
                            cmd.ElemCount,
                            glu::traits::equiv_unsigned_type_enum<ImDrawIdx>,
                            idx_buffer_offset);
                }
                idx_buffer_offset += cmd.ElemCount;
            }
        }

        // TODO: use a state tracker instead
        gl::disable(gl::scissor_test);
    }
}

