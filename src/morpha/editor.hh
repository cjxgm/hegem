#pragma once
#include "../util/constraints.hh"
#include <memory>

namespace rt::morpha
{
    struct editor final: util::constraints::only_movable
    {
        struct temporary_state;

        editor();
        ~editor(); // = default in implementation

        void draw();

        auto operator () () { return draw(); }

    private:
        std::unique_ptr<temporary_state> tmp;
    };
}

