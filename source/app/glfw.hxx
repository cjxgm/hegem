#pragma once
#include "../tool/as-czstring.hxx"
#include <functional>

namespace hegem::app::glfw
{
    auto init_once(tool::as_czstring title) -> void;
    auto mainloop_once(std::function<void()> render) -> void;
}

