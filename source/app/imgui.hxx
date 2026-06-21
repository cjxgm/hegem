#pragma once

extern "C" struct GLFWwindow;

namespace hegem::app::imgui
{
    auto init_once() -> void;
    auto on_char(GLFWwindow* win, unsigned int codepoint) -> void;
    auto on_key(GLFWwindow* win, int key, bool down) -> void;
    auto on_scroll(GLFWwindow* win, float x, float y) -> void;
    auto on_mouse_button(GLFWwindow* win, int button, bool down) -> void;
    auto on_framebuffer_resized(GLFWwindow* win, int w, int h, int fbw, int fbh) -> void;
    auto on_frame_begin(GLFWwindow* win) -> void;
    auto on_frame_end(GLFWwindow* win) -> void;
}

