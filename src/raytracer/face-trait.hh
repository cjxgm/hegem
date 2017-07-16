#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/glm/mat3.hh"
#include "../lib/glm/op/common.hh"
#include "../lib/glm/op/geom.hh"
#include "../lib/glm/op/intersect.hh"
#include "../scene/shape.hh"
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
            mesh(shapes::mesh const& m)
                : verts{m.verts.data()}
                , faces{m.faces.data()}
            {
                build_cache(m.faces.size());
            }

            using identifier_type = int;
            static constexpr int capacity() { return 8; }

            auto minmax(identifier_type face) const { return minmax_cache[face]; }
            auto pivot(identifier_type face) const { return pivot_cache[face]; }

            shape_hit_type intersect(identifier_type face, ray_type const& ray) const
            {
                auto face_attr = faces + face;
                auto& v0 = verts[face_attr->vert_ids[0]];
                auto& v1 = verts[face_attr->vert_ids[1]];
                auto& v2 = verts[face_attr->vert_ids[2]];
                auto& p0 = v0.position;
                auto& p1 = v1.position;
                auto& p2 = v2.position;
                auto& n0 = *v0.normal;
                auto& n1 = *v1.normal;
                auto& n2 = *v2.normal;
                glm::vec3 output;
                if (intersectRayTriangle(
                            ray.origin, *ray.dir,
                            p0, p1, p2,
                            output)) {
                    glm::vec3 bary{1.0f - output.x - output.y, output.x, output.y};
                    auto position = glm::mat3{p0, p1, p2} * bary;
                    auto normal = glm::mat3{n0, n1, n2} * bary;
                    auto extent = output.z;
                    return hits::shape{
                        ray,
                        extent,
                        1e-5f * extent,
                        position,
                        normal,
                    };
                } else {
                    return hits::missed{ray};
                }
            }

        private:
            vert_attributes const* verts;
            face_attributes const* faces;
            std::vector<glm::vec3> pivot_cache;
            std::vector<std::tuple<glm::vec3, glm::vec3>> minmax_cache;

            void build_cache(int face_count)
            {
                pivot_cache.reserve(face_count);
                minmax_cache.reserve(face_count);
                for (auto face_attr=faces, last=faces+face_count; face_attr < last; face_attr++) {
                    auto& p0 = verts[face_attr->vert_ids[0]].position;
                    auto& p1 = verts[face_attr->vert_ids[1]].position;
                    auto& p2 = verts[face_attr->vert_ids[2]].position;
                    auto min_pos = glm::min(p0, glm::min(p1, p2));
                    auto max_pos = glm::max(p0, glm::max(p1, p2));
                    pivot_cache.emplace_back((p0 + p1 + p2) / 3.0f);
                    minmax_cache.emplace_back(min_pos, max_pos);
                }
            }
        };
    }
}

