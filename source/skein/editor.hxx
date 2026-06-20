#pragma once
#include "../lib/glm/vec2.hxx"
#include "../tool/constraints.hxx"
#include "../scene/scene.hxx"
#include "graph.hxx"
#include <vector>
#include <memory>

namespace hegem::skein
{
    namespace editor_details
    {
        struct temporary_state;

        struct editor: tool::constraints::only_movable
        {
            scene::scene_type scene;

            editor();
            ~editor(); // = default in implementation

            // Returns true if changed
            bool draw(bool with_gizmo);
            auto force_execute(bool with_gizmo) -> void;

            auto save_toml(std::string const& path) -> void;
            auto load_toml(std::string const& path, bool with_gizmo) -> void;

        private:
            graph g;
            node_id_type previewing_node{};
            float scaling_level{};

            glm::vec2 grid_size{30.0f, 18.0f};
            glm::vec2 origin{0.0f, 0.0f};
            int default_node_width = 4;

            std::unique_ptr<temporary_state> tmp;
        };
    }

    using editor_details::editor;
}

