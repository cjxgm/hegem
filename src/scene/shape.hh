#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/mapbox/variant.hh"
#include <array>

namespace rt::scene
{
    namespace shapes
    {
        struct sphere
        {
            glm::vec3 center;
            float radius;
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
            line_segment,
            mesh
        >;
    }

    using shapes::shape_type;
}

