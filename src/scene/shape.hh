#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/mapbox/variant.hh"
#include "../math/direction.hh"
#include "../util/grid.hh"
#include "../raytracer/face-trait.hh"
#include "../hemesh/hemesh.hh"
#include "shape/mesh.hh"
#include <array>

namespace rt::scene
{
    namespace shapes
    {
        using math::direction_type;

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

        struct voxel
        {
            mesh source;
            util::grid<raytracer::face_trait::mesh> voxelized;
        };

        using rt::hemesh::hemesh;

        using shape_type = mapbox::util::variant<
            sphere,
            plane,
            voxel,
            hemesh,
            mesh
        >;
    }

    using shapes::shape_type;
}

