#pragma once
#include "../lib/glm/vec3.hxx"
#include "../lib/mapbox/variant.hxx"
#include "../math/direction.hxx"
#include "../util/grid.hxx"
#include "../raytracer/face-trait.hxx"
#include "../swing/hemesh.hxx"
#include "../kul/system.hxx"
#include "shape/mesh.hxx"
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

        using rt::swing::hemesh;
        using rt::kul::spark_system;

        using shape_type = mapbox::util::variant<
            sphere,
            plane,
            voxel,
            hemesh,
            spark_system,
            mesh
        >;
    }

    using shapes::shape_type;
}

