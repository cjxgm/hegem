#pragma once
#include "../hemesh.fwd.hh"
#include "../primitive.fwd.hh"
#include "../type.hh"

namespace rt::hemesh
{
    inline namespace op
    {
        inline namespace euler
        {
            body_type* make_body(hemesh& m, position_type pos);
            edge_type* make_edge(hemesh& m, ring_type* ring, position_type pos);
            edge_type* make_edge(hemesh& m, hege_type* hege, position_type pos);
            face_type* make_face(hemesh& m, hege_type* first, hege_type* last);
        }
    }
}

