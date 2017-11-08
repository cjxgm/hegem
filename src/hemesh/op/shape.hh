#pragma once
// basic shapes creation
#include "../../util/span.hh"
#include "../hemesh.fwd.hh"
#include "../primitive.fwd.hh"
#include "../type.hh"

namespace rt::hemesh
{
    inline namespace op
    {
        inline namespace shape
        {
            inline namespace _2d
            {
                hege_type* make_polygon(hemesh & m, util::const_span<position_type> points);
                hege_type* make_polygon_disk(hemesh & m, int ngon, float radius);
            }

            inline namespace _3d
            {
                hege_type* make_polygon_cylinder(hemesh & m, int ngon, float radius, int nseg, float height);
                hege_type* make_cube(hemesh & m);
            }
        }
    }
}

