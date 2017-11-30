#pragma once
#include "../lib/glm/vec2.hh"
#include "../util/constraints.hh"
#include "graph.hh"
#include <vector>
#include <memory>

namespace rt::sk
{
    namespace editor_details
    {
        struct temporary_state;

        struct editor: util::constraints::only_movable
        {
            editor();
            ~editor(); // = default in implementation

            void draw();

        private:
            graph g;
            node_id_type previewing_node{};
            float scaling_level{};

            glm::vec2 grid_size{30.0f, 30.0f};
            glm::vec2 origin{0.0f, 0.0f};
            int default_node_width = 5;

            std::unique_ptr<temporary_state> tmp;
        };
    }

    using editor_details::editor;
}

