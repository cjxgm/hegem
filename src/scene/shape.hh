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

        struct vert_attributes
        {
            glm::vec3 position;
            direction_type normal;
        };

        struct face_attributes
        {
            std::array<int, 3> vert_ids;

            face_attributes(int p0, int p1, int p2)
                : vert_ids{p0, p1, p2} {}
        };

        using vert_soup_type = std::vector<vert_attributes>;
        using face_soup_type = std::vector<face_attributes>;

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

        struct mesh
        {
            vert_soup_type verts;
            face_soup_type faces;
        };

        using shape_type = mapbox::util::variant<
            sphere,
            plane,
            mesh
        >;
    }

    using shapes::shape_type;
}

