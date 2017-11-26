#pragma once
#include "../lib/glm/vec2.hh"
#include "../util/constraints.hh"
#include "graph.hh"
#include <vector>

namespace rt::sk
{
    struct editor: util::constraints::only_movable
    {
        void draw_editor();
        void draw_properties();

    private:
        graph g;
        int selection;

        glm::vec2 grid_size{30.0f, 30.0f};
        glm::vec2 origin{0.0f, 0.0f};
    };
}

