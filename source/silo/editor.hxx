#pragma once
#include "../tool/constraints.hxx"
#include "../tool/channel.hxx"
#include "../tool/task-manager.hxx"
#include "../tool/desktop.hxx"
#include <memory>

namespace hegem::silo
{
    struct editor final: tool::constraints::only_movable
    {
        struct temporary_state;

        editor(tool::desktop_subsystem* desktop, int (&tile_size)[2]);
        ~editor(); // = default in implementation

        auto draw() -> void;

        auto operator () () { return draw(); }

    private:
        std::unique_ptr<temporary_state> tmp;
    };
}

