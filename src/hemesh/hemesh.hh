#pragma once
#include "primitive.hh"
#include "slab.hh"

namespace rt::hemesh
{
    struct hemesh
    {
        // Makers establishe internal data structures.
        body_type* make_body();
        face_type* make_face(body_type* body);
        vert_type* make_vert();
        ring_type* make_ring(face_type* face, vert_type* vert);
        hege_type* make_hege_twin(ring_type* ring, vert_type* vert);
        hege_type* make_hege_twin(hege_type* hege, vert_type* vert);
        edge_type* make_edge(hege_type* hege);
        bool close_hege(hege_type* h0, hege_type* h1);

    private:
        // Expands to unbound_slab<body_type> bodys; and etc.
        #define STRUCT(NAME) unbound_slab<NAME>
        #define END_STRUCT(VAR) VAR ## s;
        #include "primitive.inl"

        body_type* any_body{};
    };
}

