#include "../lib/gl/glfw.hh"
#include "../lib/gl/gl.hh"
#include "../lib/glm/vec2.hh"
#include "../utils/journal.hh"
#include "wsi.hh"
#include "ui.hh"
#include <stdexcept>
#include <string>
#include <memory>
#include <utility>  // for std::forward

#include "imgui.inl"

namespace rt::gui::wsi
{
    namespace
    {
        using rt::utils::journal;
        bool ctor_once = false;
        bool mainloop_once = false;

        journal j() { return {"WSI"}; }

        void on_error(int err, char const* desc)
        {
            throw std::runtime_error{"[WSI] glfw: error " + std::to_string(err) + ": " + desc};
        }

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
            journal{"GL"}
                << "debug:\n"
                << severity_to_ansi_escape_sequence(severity)
                << (int)source
                << "@" << (int)type
                << "#" << (int)id
                << "!" << (int)severity
                << ": " << msg
                << "\e[0m\n";
            if (severity == gl::debug_severity_notification)
                return;
            if (severity == gl::debug_severity_high
                    || type == gl::debug_type_error
                    || source == gl::debug_source_shader_compiler
                    || source == gl::debug_source_api)
                throw std::logic_error{"OpenGL error."};
        }

        template <class CallbackContext>
        struct glfw : utils::non_transferable
        {
            using callback_context_type = CallbackContext;

            glfw(gsl::cstring_span<> title)
            {
                j() << "glfw: (ctor)\n";
                if (!glfwInit()) throw std::runtime_error{"[WSI] glfw: init failed."};
                glfwSetErrorCallback(on_error);

                j() << "glfw: requesting OpenGL 4.5 Core Profile context\n";
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
                win = glfwCreateWindow(16*50, 9*50, ensure_z(title).data(), nullptr, nullptr);
                if (win == nullptr) throw std::runtime_error{"[WSI] glfw: create context failed."};

                j() << "glfw: making context current\n";
                glfwMakeContextCurrent(win);

                j() << "gl: loading\n";
                gl::load();
                j() << "gl: version info\n"
                    << "  - OpenGL:   " << (char const*)gl::get_string(gl::version) << "\n"
                    << "  - Vendor:   " << (char const*)gl::get_string(gl::vendor) << "\n"
                    << "  - Renderer: " << (char const*)gl::get_string(gl::renderer) << "\n"
                    << "  - Shader:   " << (char const*)gl::get_string(gl::shading_language_version) << "\n"
                ;

                j() << "glfw: enabling vsync\n";
                glfwSwapInterval(1);

                j() << "gl: setting up debug\n";
                gl::enable(gl::debug_output_synchronous);
                gl::debug_message_callback(on_gl_debug, nullptr);
                gl::debug_message_control(gl::dont_care, gl::dont_care, gl::dont_care, 0, nullptr, true);

                j() << "glfw: setup window dimension limits\n";
                glfwSetWindowSizeLimits(win, 16, 16, GLFW_DONT_CARE, GLFW_DONT_CARE);

                j() << "glfw: installing callbacks\n";
                glfwSetCharCallback(win, on_char);
                glfwSetKeyCallback(win, on_key);
                glfwSetScrollCallback(win, on_scroll);
                glfwSetMouseButtonCallback(win, on_mouse_button);
                glfwSetFramebufferSizeCallback(win, on_framebuffer_resized);
            }

            ~glfw()
            {
                j() << "glfw: (dtor)\n";
                if (win) glfwDestroyWindow(win);
                glfwTerminate();
            }

            template <class F>
            void mainloop(F&& render)
            {
                j() << "glfw: entering mainloop\n";
                on_framebuffer_resized(win, -1, -1);    // trigger first-time size
                while (!glfwWindowShouldClose(win)) {
                    glfwPollEvents();
                    on_frame_begin(win, *this);
                    render();
                    on_frame_end(win);
                    glfwSwapBuffers(win);
                }
                j() << "glfw: exited mainloop\n";
            }

            void register_callback_context(callback_context_type& cbctx)
            {
                j() << "glfw: registering callback context\n";
                if (glfwGetWindowUserPointer(win))
                    throw std::logic_error{"Duplicate registration of callback context."};
                glfwSetWindowUserPointer(win, &cbctx);
            }

            double now() const { return glfwGetTime(); }
            bool focused() const { return glfwGetWindowAttrib(win, GLFW_FOCUSED); }
            glm::vec2 cursor_position() const
            {
                double x, y;
                glfwGetCursorPos(win, &x, &y);
                return {x, y};
            }
            bool mouse_button_is_down(int button) const { return glfwGetMouseButton(win, button); }

        private:
            GLFWwindow* win{};

            static auto& callback_context(GLFWwindow* win)
            {
                auto p = glfwGetWindowUserPointer(win);
                if (p == nullptr) throw std::logic_error{"Callback context not registered."};
                return *static_cast<callback_context_type*>(p);
            }

            static void on_char(GLFWwindow* win, unsigned int codepoint)
            {
                callback_context(win).on_char(codepoint);
            }

            static void on_key(GLFWwindow* win, int key, int /*scancode*/, int action, int /*mods*/)
            {
                auto& cbctx = callback_context(win);
                unsigned int mods{};
                mods |= (
                            glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) ||
                            glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL)
                        ) << 0;
                mods |= (
                            glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) ||
                            glfwGetKey(win, GLFW_KEY_RIGHT_SHIFT)
                        ) << 1;
                mods |= (
                            glfwGetKey(win, GLFW_KEY_LEFT_ALT) ||
                            glfwGetKey(win, GLFW_KEY_RIGHT_ALT)
                        ) << 2;
                mods |= (
                            glfwGetKey(win, GLFW_KEY_LEFT_SUPER) ||
                            glfwGetKey(win, GLFW_KEY_RIGHT_SUPER)
                        ) << 3;
                switch (action) {
                    case GLFW_PRESS:
                        cbctx.on_key(key, true, mods);
                        break;
                    case GLFW_RELEASE:
                        cbctx.on_key(key, false, mods);
                        break;
                    case GLFW_REPEAT:
                        // ignored intentionally
                        break;
                    default: throw std::logic_error{"unreachable"};
                }
            }

            static void on_scroll(GLFWwindow* win, double x, double y)
            {
                callback_context(win).on_scroll(x, y);
            }

            static void on_mouse_button(GLFWwindow* win, int button, int action, int /*mods*/)
            {
                auto& cbctx = callback_context(win);
                switch (action) {
                    case GLFW_PRESS:
                        cbctx.on_mouse_button(button, true);
                        break;
                    case GLFW_RELEASE:
                        cbctx.on_mouse_button(button, false);
                        break;
                    default: throw std::logic_error{"unreachable"};
                }
            }

            static void on_framebuffer_resized(GLFWwindow* win, int /*fbw*/, int /*fbh*/)
            {
                int w, h, fbw, fbh;
                glfwGetWindowSize(win, &w, &h);
                glfwGetFramebufferSize(win, &fbw, &fbh);
                callback_context(win).on_framebuffer_resized(w, h, fbw, fbh);
            }

            template <class Context>
            static void on_frame_begin(GLFWwindow* win, Context& ctx)
            {
                callback_context(win).on_frame_begin(ctx);
            }

            static void on_frame_end(GLFWwindow* win)
            {
                callback_context(win).on_frame_end();
            }
        };

        std::unique_ptr<glfw<imgui_impl>> impl;
        std::unique_ptr<imgui_impl> imgui;
        std::unique_ptr<gui::ui> ui;

        inline namespace utils
        {
            template <class T, class ...Args>
            void emplace_unique(std::unique_ptr<T>& p, Args&&... args)
            {
                p = std::make_unique<T>(std::forward<Args>(args)...);
            }
        }
    }

    context::context(gsl::cstring_span<> title)
    {
        if (ctor_once) throw std::logic_error{"Cannot construct twice."};
        ctor_once = true;

        emplace_unique(impl, title);
        emplace_unique(imgui);
        ui = std::unique_ptr<gui::ui>{new gui::ui};

        impl->register_callback_context(*imgui);
    }

    context::~context()
    {
        ui.reset();
        imgui.reset();
        impl.reset();
    }

    void context::mainloop()
    {
        if (mainloop_once) throw std::logic_error{"Cannot run mainloop twice."};
        mainloop_once = true;

        impl->mainloop([&] () {
            ui->render();
        });
    }
}

