#pragma once
#include "../../lib/glm/vec3.hh"
#include "../../math/direction.hh"
#include <array>
#include <vector>

namespace rt::scene
{
    namespace shapes
    {
        using math::direction_type;

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

        struct mesh
        {
            vert_soup_type verts;
            face_soup_type faces;
        };

        void extend(mesh & dst, mesh const& src);
    }
}

