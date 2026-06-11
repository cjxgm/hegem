#pragma once
// basic shapes creation
#include "../../util/span.hxx"
#include "../hemesh.fwd.hxx"
#include "../primitive.fwd.hxx"
#include "../type.hxx"

namespace rt::swing
{
    inline namespace op
    {
        inline namespace shape
        {
            inline namespace _2d
            {
                // make_polygon(m, p, nullptr, nullptr)
                //      Make a new body with a 2-sided polygon
                //
                // make_polygon(m, p, f, nullptr)
                //      Make a polygon ring inside f
                //      with a 1-sided face covering the ring
                //
                // make_polygon(m, p, f, cf)
                //      Make a polygon ring inside f
                //      with another ring inside cf,
                //      so that the 2 new rings become a hole in f and cf.
                hege_type* make_polygon(hemesh & m, util::const_span<position_type> points, face_type* outer_face=nullptr, face_type* counter_face=nullptr);
                hege_type* make_polygon_disk(hemesh & m, int ngon, float radius, face_type* outer_face=nullptr, face_type* counter_face=nullptr);
            }

            inline namespace _3d
            {
                hege_type* make_polygon_cylinder(hemesh & m, int ngon, float radius, int nseg, float height);
                hege_type* make_cube(hemesh & m);
            }
        }
    }
}

