#include "../lib/gl/glfw.hh"
#include "../lib/gl/gl.hh"
#include "../util/journal.hh"
#include "glfw.hh"
#include "imgui.hh"
#include <stdexcept>
#include <string>
#include <utility>  // for std::move

namespace rt::app::glfw
{
    namespace
    {
        using rt::util::journal;
        journal j() { return {"WSI"}; }

        GLFWwindow* win{};

        char const* severity_to_ansi_escape_sequence(gl::enum_type severity)
        {
            switch (severity) {
                case gl::debug_severity_notification: return "\e[0;36m";
                case gl::debug_severity_low: return "\e[0;33m";
                case gl::debug_severity_medium: return "\e[0;35m";
                case gl::debug_severity_high: return "\e[1;31m";
                default: throw std::logic_error{"unreachable"};
            }
        }

        inline namespace callbacks
        {
            void on_gl_debug(
                    gl::enum_type source,
                    gl::enum_type type,
                    gl::uint_type id,
                    gl::enum_type severity,
                    gl::sizei_type /*length*/,
                    gl::char_type const* msg,
                    void const* /*userdata*/)
            {
                if (id == 131185) return;
                if (id == 131169) return;
                if (id == 131140) return;
                journal{"GL"}
                    << "debug:\n"
                    << severity_to_ansi_escape_sequence(severity)
                    << (int)source
                    << "@" << (int)type
                    << "#" << (int)id
                    << "!" << (int)severity
                    << ": " << msg
                    << "\e[0m\n";
                if (id == 131218) return;
                if (severity == gl::debug_severity_notification)
                    return;
                if (severity == gl::debug_severity_high
                        || type == gl::debug_type_error
                        || source == gl::debug_source_shader_compiler
                        || source == gl::debug_source_api)
                    throw std::logic_error{"OpenGL error."};
            }

            void on_error(int err, char const* desc)
            {
                throw std::runtime_error{"[WSI] glfw: error " + std::to_string(err) + ": " + desc};
            }

            void on_char(GLFWwindow* win, unsigned int codepoint)
            {
                imgui::on_char(win, codepoint);
            }

            void on_key(GLFWwindow* win, int key, int /*scancode*/, int action, int /*mods*/)
            {
                switch (action) {
                    case GLFW_PRESS:
                        imgui::on_key(win, key, true);
                        break;
                    case GLFW_RELEASE:
                        imgui::on_key(win, key, false);
                        break;
                    case GLFW_REPEAT:
                        // ignored intentionally
                        break;
                    default: throw std::logic_error{"unreachable"};
                }
            }

            void on_scroll(GLFWwindow* win, double x, double y)
            {
                imgui::on_scroll(win, x, y);
            }

            void on_mouse_button(GLFWwindow* win, int button, int action, int /*mods*/)
            {
                switch (action) {
                    case GLFW_PRESS:
                        imgui::on_mouse_button(win, button, true);
                        break;
                    case GLFW_RELEASE:
                        imgui::on_mouse_button(win, button, false);
                        break;
                    default: throw std::logic_error{"unreachable"};
                }
            }

            void on_framebuffer_resized(GLFWwindow* win, int /*fbw*/, int /*fbh*/)
            {
                int w, h, fbw, fbh;
                glfwGetWindowSize(win, &w, &h);
                glfwGetFramebufferSize(win, &fbw, &fbh);
                imgui::on_framebuffer_resized(win, w, h, fbw, fbh);
            }

            void on_frame_begin(GLFWwindow* win)
            {
                imgui::on_frame_begin(win);
            }

            void on_frame_end(GLFWwindow* win)
            {
                imgui::on_frame_end(win);
            }
        }

        struct context
        {
            context(util::as_czstring title)
            {
                j() << "glfw: (ctor)\n";
                if (!glfwInit()) throw std::runtime_error{"[WSI] glfw: init failed."};
                glfwSetErrorCallback(on_error);

                j() << "glfw: requesting OpenGL 4.5 Core Profile context, windowed fullscreen mode\n";
                auto monitor = glfwGetPrimaryMonitor();
                auto& mode = *glfwGetVideoMode(monitor);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
                glfwWindowHint(GLFW_AUTO_ICONIFY, false);
                glfwWindowHint(GLFW_RED_BITS, mode.redBits);
                glfwWindowHint(GLFW_GREEN_BITS, mode.greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, mode.blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, mode.refreshRate);
                win = glfwCreateWindow(mode.width, mode.height, title.data(), monitor, nullptr);
                if (win == nullptr) throw std::runtime_error{"[WSI] glfw: create context failed."};

                j() << "glfw: making context current\n";
                glfwMakeContextCurrent(win);

                j() << "gl: loading\n";
                gl::load();
                j() << "gl: version info\n"
                    << " - OpenGL:   " << (char const*)gl::get_string(gl::version) << "\n"
                    << " - Vendor:   " << (char const*)gl::get_string(gl::vendor) << "\n"
                    << " - Renderer: " << (char const*)gl::get_string(gl::renderer) << "\n"
                    << " - Shader:   " << (char const*)gl::get_string(gl::shading_language_version) << "\n"
                ;

                j() << "glfw: enabling vsync\n";
                glfwSwapInterval(1);

                j() << "gl: setting up debug\n";
                gl::enable(gl::debug_output_synchronous);
                gl::debug_message_callback(on_gl_debug, nullptr);
                gl::debug_message_control(gl::dont_care, gl::dont_care, gl::dont_care, 0, nullptr, true);

                j() << "glfw: setup window dimension limits\n";
                glfwSetWindowSizeLimits(win, 16, 16, GLFW_DONT_CARE, GLFW_DONT_CARE);
            }

            ~context()
            {
                j() << "glfw: (dtor) \e[1;32m-- must be the last output --\e[0m\n";
                if (win) glfwDestroyWindow(win);
                glfwTerminate();
            }
        };

        struct context_mainloop
        {
            template <class Render>
            context_mainloop(Render&& render)
            {
                j() << "glfw mainloop: init imgui\n";
                imgui::init_once();
                j() << "glfw mainloop: installing callbacks\n";
                glfwSetCharCallback(win, on_char);
                glfwSetKeyCallback(win, on_key);
                glfwSetScrollCallback(win, on_scroll);
                glfwSetMouseButtonCallback(win, on_mouse_button);
                glfwSetFramebufferSizeCallback(win, on_framebuffer_resized);
                on_framebuffer_resized(win, -1, -1);    // trigger first-time size

                j() << "glfw mainloop: entering\n";
                while (!glfwWindowShouldClose(win)) {
                    glfwPollEvents();
                    on_frame_begin(win);
                    render();
                    on_frame_end(win);
                    glfwSwapBuffers(win);
                }
                j() << "glfw mainloop: exited\n";
            }
        };
    }

    void init_once(util::as_czstring title)
    {
        static context _{title};
    }

    void mainloop_once(std::function<void()> render)
    {
        static context_mainloop _{std::move(render)};
    }
}

