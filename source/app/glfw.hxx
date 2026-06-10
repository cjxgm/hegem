#pragma once
#include "../util/as-czstring.hxx"
#include <functional>

namespace rt::app::glfw
{
    void init_once(util::as_czstring title);
    void mainloop_once(std::function<void()> render);
}

