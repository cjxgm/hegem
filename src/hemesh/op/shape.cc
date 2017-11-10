#include "../../math/constants.hh"
#include "../primitive.hh"
#include "shape.hh"
#include "euler.hh"
#include "sweep.hh"
#include <stdexcept>
#include <vector>
#include <cmath>

namespace rt::hemesh
{
    inline namespace op
    {
        inline namespace shape
        {
            inline namespace _2d
            {
                hege_type* make_polygon(hemesh & m, util::const_span<position_type> points)
                {
                    if (points.size() < 3) {
                        throw std::invalid_argument{
                            "At least 3 points are required "
                            "in order to form a triangle (3-polygon)"
                        };
                    }

                    auto r = make_body(m, points[0])->any_face->boundary;
                    auto front = make_edge(m, r, points[1])->any_hege;

                    auto back = front;
                    for (auto& p: points.range(2)) {
                        back = make_edge(m, back, p)->any_hege;
                    }

                    make_face(m, front, back);
                    return front;
                }

                hege_type* make_polygon_disk(hemesh & m, int ngon, float radius)
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

                    return make_polygon(m, points);
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
                    auto r = h->ring;
                    offset_type offset{0.0f, height / float(nseg), 0.0f};
                    while (nseg-- > 0) {
                        extrude(m, r, offset);
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

