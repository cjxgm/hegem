#pragma once
#include "../hemesh.fwd.hxx"
#include "../primitive.fwd.hxx"
#include "../type.hxx"

namespace hegem::swing
{
    inline namespace op
    {
        inline namespace euler
        {
            body_type* make_body(hemesh& m, position_type pos);
            edge_type* make_edge(hemesh& m, ring_type* ring, position_type pos);
            edge_type* make_edge(hemesh& m, hege_type* hege, position_type pos);
            face_type* make_face(hemesh& m, hege_type* first, hege_type* last);
            ring_type* kill_bridge(hemesh& m, hege_type* bridge_to_inner);
            ring_type* face_to_ring(hemesh& m, face_type* inner_face, face_type* outer_face);
            edge_type* split_edge(hemesh& m, edge_type* edge, position_type pos);
        }

        inline namespace euler_reverse
        {
            hege_type* bridge(hemesh& m, hege_type* p0, hege_type* p1);
        }
    }
}

