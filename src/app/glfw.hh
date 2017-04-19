#pragma once
#include "../util/as-czstring.hh"
#include <functional>

namespace rt::app::glfw
{
    void init_once(util::as_czstring title);
    void mainloop_once(std::function<void()> render);
}

