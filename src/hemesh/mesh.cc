#include "../lib/mapbox/earcut.hh"
#include "../lib/glm/mat3.hh"
#include "../math/local-space.hh"
#include "mesh.hh"
#include "hemesh.hh"
#include "list.hh"
#include "geometry.hh"
#include <vector>
#include <array>

namespace rt::hemesh
{
    namespace
    {
        using list::iterate;
        using earcut_point_type = std::array<float, 2>;
        using earcut_ring_type = std::vector<earcut_point_type>;
        using earcut_polygon_type = std::vector<earcut_ring_type>;

        struct projector_2d
        {
            projector_2d(direction_type normal) : space{math::local_space(normal)} {}

            earcut_point_type operator () (position_type p) const
            {
                return {
                    dot(p, space[0]),
                    dot(p, space[1]),
                };
            }

        private:
            glm::mat3 space;
        };

        scene::shapes::mesh triangulate_face(face_type* face)
        {
            auto n = normal(face->boundary->any_hege);
            scene::shapes::mesh tri_mesh;

            // Project to 2D and populate vert attributes
            earcut_polygon_type polygon;
            {
                projector_2d project{n};
                for (auto& r: iterate(face->boundary)) {
                    polygon.emplace_back();
                    auto& er = polygon.back();

                    for (auto& h: iterate(r.any_hege)) {
                        auto v = h.start;
                        tri_mesh.verts.push_back({ v->pos, n });
                        er.emplace_back(project(v->pos));
                    }
                }
            }

            // Triangulate and populate face attributes
            auto tris = mapbox::earcut<int>(polygon);
            for (auto last=&*end(tris), p=&*begin(tris); p<last; p+=3)
                tri_mesh.faces.emplace_back(p[0], p[1], p[2]);

            return tri_mesh;
        }
    }

    scene::shapes::mesh build_mesh(hemesh const& m)
    {
        scene::shapes::mesh tri_mesh;
        for (auto& b: iterate(m.any_body)) {
            for (auto& f: iterate(b.any_face)) {
                extend(tri_mesh, triangulate_face(&f));
            }
        }
        return tri_mesh;
    }
}

