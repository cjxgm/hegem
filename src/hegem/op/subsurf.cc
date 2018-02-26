#include "../../lib/mapbox/earcut.hh"
#include "../../lib/glm/mat3.hh"
#include "../../math/local-space.hh"
#include "../../util/range.hh"
#include "../hemesh.hh"
#include "../list.hh"
#include "../geometry.hh"
#include "../iteration.hh"
#include "euler.hh"
#include "subsurf.hh"
#include <vector>
#include <array>
#include <tuple>
#include <map>
#include <unordered_map>

namespace rt::hegem
{
    inline namespace op
    {
        inline namespace subsurf
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

                struct triangulation_cache
                {
                    std::map<std::tuple<face_type*, vert_type*, vert_type*>, hege_type*> heges;
                    std::unordered_map<vert_type*, vert_type*> vert_map;
                    body_type* body{};
                    hemesh result;
                };

                void triangulate(triangulation_cache& ca, face_type* face)
                {
                    std::vector<vert_type*> verts;
                    earcut_polygon_type polygon;

                    // Project to 2D for earcut and prepare back-mapping
                    auto n = normal(face->boundary->any_hege);
                    projector_2d project{n};
                    for (auto& r: iterate(face->boundary)) {
                        auto& er = polygon.emplace_back();
                        for (auto& h: iterate(r.any_hege)) {
                            er.emplace_back(project(h.start->pos));
                            verts.emplace_back(h.start);
                        }
                    }

                    // triangulate via earcut
                    auto tris = mapbox::earcut<int>(polygon);

                    // reconstruct vertices
                    for (auto v: verts) {
                        if (ca.vert_map.count(v) == 0) {
                            auto nv = ca.result.make_vert();
                            nv->pos = v->pos;
                            ca.vert_map.emplace(v, nv);
                        }
                    }

                    auto get_hege_or_connect = [&ca] (face_type* f, vert_type* a, vert_type* b) -> hege_type* {
                        auto it = ca.heges.find({f, a, b});
                        if (it == end(ca.heges)) {
                            auto h0 = ca.result.heges.alloc();
                            auto h1 = ca.result.heges.alloc();
                            h0->twin = h1;
                            h1->twin = h0;
                            h0->start = a;
                            h1->start = b;
                            a->any_hege = h0;
                            b->any_hege = h1;
                            ca.result.make_edge(h0);
                            ca.heges.emplace(std::tuple{f, a, b}, h0);
                            ca.heges.emplace(std::tuple{f, b, a}, h1);
                            return h0;
                        } else {
                            return it->second;
                        }
                    };

                    auto get_hege_or_connect_unambiguously = [&get_hege_or_connect] (face_type* f, vert_type* a, vert_type* b) -> hege_type* {
                        auto h = get_hege_or_connect(nullptr, a, b);
                        if (h->ring == nullptr) return h;
                        return get_hege_or_connect(f, a, b);
                    };

                    // reconstruct heges, edges, rings, faces
                    for (auto last=&*end(tris), p=&*begin(tris); p<last; p+=3) {
                        auto a = ca.vert_map[verts[p[0]]];
                        auto b = ca.vert_map[verts[p[1]]];
                        auto c = ca.vert_map[verts[p[2]]];

                        auto h0 = get_hege_or_connect_unambiguously(face, a, b);
                        auto h1 = get_hege_or_connect_unambiguously(face, b, c);
                        auto h2 = get_hege_or_connect_unambiguously(face, c, a);

                        auto face = ca.result.make_face(ca.body);
                        auto ring = ca.result.make_ring(face, h0->start);
                        ring->any_hege = h0;

                        if (h0->ring != nullptr) throw std::logic_error{"h0 taken by others"};
                        if (h1->ring != nullptr) throw std::logic_error{"h1 taken by others"};
                        if (h2->ring != nullptr) throw std::logic_error{"h2 taken by others"};

                        h0->ring = ring;
                        h1->ring = ring;
                        h2->ring = ring;

                        list::connect(h0, h1);
                        list::connect(h1, h2);
                        list::connect(h2, h0);
                    }
                }
            }

            auto triangulate(hemesh m) -> hemesh
            {
                triangulation_cache ca;

                for (auto& b: iterate(m.any_body)) {
                    ca.body = ca.result.make_body();

                    for (auto& f: iterate(b.any_face))
                        triangulate(ca, &f);
                }

                return std::move(ca.result);
            }

            void convexify(hemesh& m)
            {
                // TODO: unimplemented
                throw std::logic_error{"unimplemented"};
            }

            void subdivide_catmull_clark(hemesh& m)
            {
                std::unordered_map<face_type*, position_type> face_coords;
                for (auto& ring: m.rings.nodes) {
                    if (ring.any_hege == nullptr) continue;

                    position_type pos{};
                    float vert_count{};
                    for (auto& h: iterate(ring.any_hege)) {
                        pos += h.start->pos;
                        vert_count++;
                    }
                    pos /= vert_count;

                    face_coords.emplace(ring.face, pos);
                }

                std::unordered_map<edge_type*, position_type> edge_coords;
                for (auto& edge: m.edges.nodes) {
                    if (edge.any_hege == nullptr) continue;

                    auto h0 = edge.any_hege;
                    auto h1 = h0->twin;
                    auto f0 = h0->ring->face;
                    auto f1 = h1->ring->face;
                    auto pos = (
                        h0->start->pos + h1->start->pos +
                        face_coords[f0] + face_coords[f1]
                    ) * 0.25f;

                    edge_coords.emplace(&edge, pos);
                }

                std::unordered_map<vert_type*, position_type> vert_coords;
                for (auto& vert: m.verts.nodes) {
                    if (vert.any_hege == nullptr) continue;

                    position_type pos{};
                    float face_count{};
                    for (auto& h: iter::heges_around_vert(vert.any_hege)) {
                        pos += h.twin->start->pos;
                        pos += face_coords[h.ring->face];
                        face_count++;
                    }
                    pos /= face_count;
                    pos += (face_count - 2.0f) * vert.pos;
                    pos /= face_count;

                    vert_coords.emplace(&vert, pos);
                }

                for (auto [vert, pos]: vert_coords)
                    vert->pos = pos;

                std::unordered_multimap<face_type*, hege_type*> edge_point_on_face;
                for (auto [edge, pos]: edge_coords) {
                    auto h0 = edge->any_hege;
                    auto h1 = split_edge(m, edge, pos)->any_hege->twin;
                    edge_point_on_face.emplace(h0->ring->face, h0);
                    edge_point_on_face.emplace(h1->ring->face, h1);
                }

                for (auto [face, pos]: face_coords) {
                    auto [it, last] = edge_point_on_face.equal_range(face);
                    if (it == last) continue;

                    auto barycenter_vert = make_edge(m, it->second, pos)->any_hege->twin->start;
                    for (auto [_, hege_to_edge_point]: util::range{++it, last}) {
                        (void)_;

                        auto first = [&] {
                            for (auto& h: iterate(hege_to_edge_point))
                                if (h.start == barycenter_vert)
                                    return &h;
                            throw std::logic_error{"Cannot reach the barycenter vertex."};
                        } ();

                        make_face(m, first, hege_to_edge_point);
                    }
                }
            }
        }
    }
}

