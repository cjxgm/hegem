#pragma once
#include "../lib/glm/vec3.hh"
#include "../scene/shape.hh"
#include "../util/bvh.hh"
#include "ray.hh"
#include "hit.hh"
#include <vector>
#include <tuple>

namespace rt::raytracer
{
    namespace face_trait
    {
        using scene::shapes::vert_attributes;
        using scene::shapes::face_attributes;
        namespace shapes = scene::shapes;

        struct mesh
        {
            mesh(shapes::mesh const& m);

            using identifier_type = int;
            static constexpr int capacity() { return 8; }

            auto minmax(identifier_type face) const { return minmax_cache[face]; }
            auto pivot(identifier_type face) const { return pivot_cache[face]; }

            shape_hit_type intersect(identifier_type face, ray_type const& ray) const;

        private:
            vert_attributes const* verts;
            face_attributes const* faces;
            std::vector<glm::vec3> pivot_cache;
            std::vector<std::tuple<glm::vec3, glm::vec3>> minmax_cache;

            void build_cache(int face_count);
        };
    }
}

namespace rt::util::bvh_details
{
    extern template struct bvh<raytracer::face_trait::mesh>;
}

