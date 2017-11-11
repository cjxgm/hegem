#include "../../math/constants.hh"
#include "../primitive.hh"
#include "shape.hh"
#include "euler.hh"
#include "sweep.hh"
#include <utility>      // for std::swap
#include <stdexcept>
#include <vector>
#include <cmath>

namespace rt::hegem
{
    inline namespace op
    {
        inline namespace shape
        {
            inline namespace _2d
            {
                hege_type* make_polygon(hemesh & m, util::const_span<position_type> points, face_type* outer_face, face_type* counter_face)
                {
                    if (points.size() < 3) {
                        throw std::invalid_argument{
                            "At least 3 points are required "
                            "in order to form a triangle (3-polygon)"
                        };
                    }

                    if (outer_face == nullptr && counter_face != nullptr) {
                        throw std::invalid_argument{
                            "Both outer face and counter face are required "
                            "to make a hole."
                        };
                    }

                    auto fill_face = [&] (hege_type* front) {
                        auto back = front;
                        for (auto& p: points.range(2))
                            back = make_edge(m, back, p)->any_hege;
                        make_face(m, front, back);
                    };

                    if (outer_face) {
                        auto bridge = make_edge(m, outer_face->boundary->any_hege, points[0])->any_hege;
                        auto front = make_edge(m, bridge, points[1])->any_hege;

                        fill_face(front);
                        kill_bridge(m, bridge);

                        if (counter_face)
                            face_to_ring(m, front->ring->face, counter_face);

                        return front;
                    } else {
                        auto r = make_body(m, points[0])->any_face->boundary;
                        auto front = make_edge(m, r, points[1])->any_hege;
                        fill_face(front);
                        return front;
                    }
                }

                hege_type* make_polygon_disk(hemesh & m, int ngon, float radius, face_type* outer_face, face_type* counter_face)
                {
                    if (ngon < 3) {
                        throw std::invalid_argument{
                            "At least 3 points are required "
                            "in order to form a triangle (3-polygon)"
                        };
                    }

                    if (radius < 0) radius = -radius;

                    std::vector<position_type> points;
                    points.reserve(ngon);
                    for (auto count=float(ngon), i=0.0f; i<count; i++) {
                        auto a = -2.0f * math::pi * i / count;
                        points.emplace_back(std::cos(a) * radius, 0.0f, std::sin(a) * radius);
                    }

                    return make_polygon(m, points, outer_face, counter_face);
                }
            }

            inline namespace _3d
            {
                hege_type* make_polygon_cylinder(hemesh & m, int ngon, float radius, int nseg, float height)
                {
                    if (nseg < 1) {
                        throw std::invalid_argument{
                            "At least 1 segment required."
                        };
                    }

                    auto h = make_polygon_disk(m, ngon, radius);
                    auto f = h->ring->face;
                    offset_type offset{0.0f, height / float(nseg), 0.0f};
                    while (nseg-- > 0) {
                        extrude(m, f, offset);
                    }

                    return h;
                }

                hege_type* make_cube(hemesh & m)
                {
                    return make_polygon_cylinder(m, 4, math::inv_sqrt_2, 1, 1.0f);
                }
            }
        }
    }
}

