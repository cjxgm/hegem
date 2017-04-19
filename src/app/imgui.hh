#pragma once

extern "C" struct GLFWwindow;

namespace rt::app::imgui
{
    void init_once();
    void on_char(GLFWwindow* win, unsigned int codepoint);
    void on_key(GLFWwindow* win, int key, bool down);
    void on_scroll(GLFWwindow* win, float x, float y);
    void on_mouse_button(GLFWwindow* win, int button, bool down);
    void on_framebuffer_resized(GLFWwindow* win, int w, int h, int fbw, int fbh);
    void on_frame_begin(GLFWwindow* win);
    void on_frame_end(GLFWwindow* win);
}

