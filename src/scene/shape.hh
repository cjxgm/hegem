#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/mapbox/variant.hh"
#include "../math/unit.hh"
#include <array>

namespace rt::scene
{
    namespace shapes
    {
        using direction_type = math::unit<glm::vec3>;

        struct sphere
        {
            glm::vec3 center;
            float radius;
        };

        struct plane
        {
            direction_type normal;
            float offset;
        };

        struct line_segment
        {
            std::array<glm::vec3, 2> points;
        };

        struct mesh
        {
            // TODO
        };

        using shape_type = mapbox::util::variant<
            sphere,
            plane,
            line_segment,
            mesh
        >;
    }

    using shapes::shape_type;
}

