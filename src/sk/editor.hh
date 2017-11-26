#pragma once
#include "../lib/glm/vec2.hh"
#include "../util/constraints.hh"
#include "graph.hh"
#include <vector>

namespace rt::sk
{
    struct editor: util::constraints::only_movable
    {
        void draw();

        struct draw_state
        {
            int new_node_x{};
            int new_node_y{};
            int new_node_w{};
        };

    private:
        graph g;
        node_id_type selection;

        glm::vec2 grid_size{30.0f, 30.0f};
        glm::vec2 origin{0.0f, 0.0f};
        int default_node_width = 5;

        draw_state state;
    };
}

