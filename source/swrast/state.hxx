#pragma once
#include "../lib/glm/vec2.hxx"
#include "../lib/glm/vec3.hxx"
#include "../math/rat.hxx"
#include "../math/direction.hxx"
#include "../scene/scene.hxx"
#include <set>
#include <vector>

namespace rt::swrast
{
    namespace state_details
    {
        using dimension_type = glm::ivec2;
        using screen_coord_type = glm::ivec2;
        using point_type = glm::vec3;
        using polygon_id_type = int;
        using math::direction_type;
        using scene::scene_type;
        using scene::view_type;

        struct edge_descriptor
        {
            math::rat<int> x;
            math::rat<int> dx;
            int y_front;
            int y_back;
            polygon_id_type id;

            edge_descriptor(polygon_id_type id, screen_coord_type lower, screen_coord_type upper);
        };

        struct polygon_descriptor
        {
            polygon_descriptor(point_type p0, point_type p1, point_type p2);
            auto z_at(point_type p) const -> float;

        private:
            // Assume P=(x, y, z), plane equation: dot(P, normal) = d
            direction_type normal;
            float d;
        };

        struct inactive_edge_order
        {
            auto operator () (edge_descriptor const& a, edge_descriptor const& b) const -> bool
            {
                return (a.y_front < b.y_front);
            }
        };

        using edge_list_type = std::multiset<edge_descriptor, inactive_edge_order>;
        using polygon_list_type = std::vector<polygon_descriptor>;

        struct state
        {
            edge_list_type edges;
            polygon_list_type polys;
            dimension_type size;

            state(scene_type const& s, view_type view);
        };
    }

    using state_details::state;
    using state_details::edge_descriptor;
    using state_details::polygon_descriptor;
    using state_details::polygon_id_type;
    using state_details::edge_list_type;
    using state_details::polygon_list_type;
    using state_details::dimension_type;
    using state_details::point_type;
    using state_details::screen_coord_type;
}

