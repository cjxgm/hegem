#pragma once
#include "../tool/as-czstring.hxx"
#include <functional>

namespace hegem::app::glfw
{
    void init_once(tool::as_czstring title);
    void mainloop_once(std::function<void()> render);
}

