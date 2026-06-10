#pragma once
#include "type.hxx"
#include "primitive.fwd.hxx"

namespace rt::hegem
{
    inline namespace geometry_operator
    {
        direction_type normal(
            hege_type* hege,
            float eps=1e-6f,
            direction_type fallback = offset_type{0, 1, 0});
        bool is_same_side(
            direction_type dir0,
            direction_type dir1,
            float eps=1e-3f);
        bool is_same_side(
            direction_type normal, position_type origin,    // a plane
            position_type point,
            float eps=1e-3f);
    }
}

