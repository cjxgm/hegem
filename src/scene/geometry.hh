#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/mapbox/variant.hh"
#include <array>

namespace rt::scene
{
    namespace geometrys
    {
        struct sphere
        {
            const glm::vec3 center;
            const float radius;
        };

        struct line_segment
        {
            const std::array<glm::vec3, 2> points;
        };

        struct mesh
        {
            // TODO
        };

        using geometry_type = mapbox::util::variant<
            sphere,
            line_segment,
            mesh
        >;
    }

    using geometrys::geometry_type;
}

