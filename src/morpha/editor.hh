#pragma once
#include "../util/constraints.hh"
#include "../util/channel.hh"
#include "../util/task-manager.hh"
#include <memory>

namespace rt::morpha
{
    struct editor final: util::constraints::only_movable
    {
        struct temporary_state;

        editor(int (&tile_size)[2]);
        ~editor(); // = default in implementation

        void draw();

        auto operator () () { return draw(); }

    private:
        std::unique_ptr<temporary_state> tmp;
    };
}

