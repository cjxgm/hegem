#include "../lib/gl/glfw.hh"
#include "../lib/gl/gl.hh"
#include "../utils/journal.hh"
#include "wsi.hh"
#include "ui.hh"
#include <stdexcept>
#include <string>
#include <memory>

using rt::utils::journal;

namespace rt::gui::wsi
{
    namespace
    {
        bool ctor_once = false;
        bool mainloop_once = false;

        journal j() { return {"WSI"}; }

        void on_error(int err, char const* desc)
        {
            throw std::runtime_error{"[WSI] glfw: error " + std::to_string(err) + ": " + desc};
        }

        struct glfw : utils::non_transferable
        {
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
                win = glfwCreateWindow(16*50, 9*50, ensure_z(title).data(), nullptr, nullptr);
                if (win == nullptr) throw std::runtime_error{"[WSI] glfw: create context failed."};

                j() << "glfw: making context current\n";
                glfwMakeContextCurrent(&*win);

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
            }

            ~glfw()
            {
                j() << "glfw: (dtor)\n";
                if (win != nullptr) glfwDestroyWindow(win);
                glfwTerminate();
            }

            template <class F>
            void mainloop(F&& render)
            {
                j() << "glfw: entering mainloop\n";
                while (!glfwWindowShouldClose(&*win)) {
                    glfwPollEvents();
                    render();
                    glfwSwapBuffers(&*win);
                }
                j() << "glfw: exited mainloop\n";
            }

        private:
            GLFWwindow* win{};
        };

        std::unique_ptr<glfw> impl;
        std::unique_ptr<gui::ui> ui;
    }

    context::context(gsl::cstring_span<> title)
    {
        if (ctor_once) throw std::logic_error{"Cannot construct twice."};
        ctor_once = true;

        impl = std::make_unique<glfw>(title);
        ui = std::unique_ptr<gui::ui>{new gui::ui};
    }

    context::~context()
    {
        ui.reset();
        impl.reset();
    }

    void context::mainloop()
    {
        if (mainloop_once) throw std::logic_error{"Cannot run mainloop twice."};
        mainloop_once = true;

        impl->mainloop([&, this] () {
            ui->render(*this);
        });
    }
}

