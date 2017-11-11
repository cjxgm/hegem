#include "../lib/mapbox/earcut.hh"
#include "../lib/glm/mat3.hh"
#include "../math/local-space.hh"
#include "mesh.hh"
#include "hemesh.hh"
#include "list.hh"
#include "geometry.hh"
#include <vector>
#include <array>

namespace rt::hegem
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

        scene::shapes::mesh outline_hege(hege_type* hege, direction_type normal)
        {
            constexpr auto width = 0.02f;
            constexpr auto arrow_center_sliding = 0.52f;
            constexpr auto arrow_head_length = 0.10f;
            constexpr auto arrow_tail_length = 0.01f;
            constexpr auto arrow_width = 0.05f;

            scene::shapes::mesh tri_mesh;
            auto p0 = hege->start->pos;
            auto p1 = hege->twin->start->pos;

            // Build local axis
            //      z: normal
            //      y: the same direction as hege
            //      x: cross(y, z)
            auto y = direction_type{p1 - p0};
            auto x = direction_type{cross(*y, *normal)};

            // hege
            tri_mesh.verts.push_back({ p0,             normal });   // 0
            tri_mesh.verts.push_back({ p1,             normal });   // 1
            tri_mesh.verts.push_back({ p0 - x * width, normal });   // 2
            tri_mesh.verts.push_back({ p1 - x * width, normal });   // 3
            tri_mesh.faces.emplace_back(0, 1, 2);
            tri_mesh.faces.emplace_back(3, 2, 1);

            // No arrow if too close
            if (distance(p0, p1) < 0.1f) return tri_mesh;

            // arrow
            auto arrow_center = mix(p0, p1, arrow_center_sliding) - x * width;
            auto arrow_head = arrow_center + y * arrow_head_length;
            auto arrow_tail = arrow_center - y * arrow_tail_length - x * arrow_width;
            tri_mesh.verts.push_back({ arrow_center, normal });     // 4
            tri_mesh.verts.push_back({ arrow_head,   normal });     // 5
            tri_mesh.verts.push_back({ arrow_tail,   normal });     // 6
            tri_mesh.faces.emplace_back(4, 5, 6);

            return tri_mesh;
        }

        scene::shapes::mesh outline_face(face_type* face, float bias)
        {
            scene::shapes::mesh tri_mesh;
            auto n = normal(face->boundary->any_hege);
            auto offset = n * bias;

            for (auto& r: iterate(face->boundary))
                for (auto& h: iterate(r.any_hege))
                    extend(tri_mesh, outline_hege(&h, n));

            for (auto& v: tri_mesh.verts)
                v.position += offset;

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

    scene::shapes::mesh build_outline_mesh(hemesh const& m, float bias)
    {
        scene::shapes::mesh tri_mesh;
        for (auto& b: iterate(m.any_body)) {
            for (auto& f: iterate(b.any_face)) {
                extend(tri_mesh, outline_face(&f, bias));
            }
        }
        return tri_mesh;
    }
}

