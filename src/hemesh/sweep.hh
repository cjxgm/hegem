#pragma once
#include "type.hh"
#include "hemesh.fwd.hh"
#include "primitive.fwd.hh"

namespace rt::hemesh
{
    inline namespace sweep_operator
    {
        void extrude(hemesh & m, ring_type* ring, offset_type offset, float eps=1e-3f);
    }
}

