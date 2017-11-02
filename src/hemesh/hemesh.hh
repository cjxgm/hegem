#pragma once
#include "primitive.hh"
#include "slab.hh"
#include "list.hh"

namespace rt::hemesh
{
    struct hemesh
    {
        // makers.
        //
        // All makers establish topological connections.
        auto* make_body()
        {
            return list::append(any_body, bodys.alloc());
        }

        auto* make_face(body_type* body)
        {
            auto* f = list::append(body->any_face, faces.alloc());
            f->body = body;
            return f;
        }

        auto* make_vert()
        {
            return verts.alloc();
        }

        auto* make_ring(face_type* face, vert_type* vert)
        {
            auto* r = list::append(face->boundary, rings.alloc());
            r->face = face;
            r->any_vert = vert;
            return r;
        }

        hege_type* make_hege_twin(ring_type* ring, vert_type* vert)
        {
            if (ring->any_hege) return nullptr;

            auto* h0 = list::append(ring->any_hege, heges.alloc());
            auto* h1 = list::append(ring->any_hege, heges.alloc());
            h0->twin = h1;
            h1->twin = h0;
            h0->ring = ring;
            h1->ring = ring;
            h0->start = ring->any_vert;
            h1->start = vert;
            h0->start->any_hege = h0;
            h1->start->any_hege = h1;
            return h0;
        }

        hege_type* make_hege_twin(hege_type* hege, vert_type* vert)
        {
            if (!hege->ring->any_hege) return nullptr;

            auto* h0 = list::insert_after(hege, heges.alloc());
            auto* h1 = list::insert_after(h0,   heges.alloc());
            h0->twin = h1;
            h1->twin = h0;
            h0->ring = hege->ring;
            h1->ring = hege->ring;
            h0->start = hege->twin->start;
            h1->start = vert;
            h0->start->any_hege = h0;
            h1->start->any_hege = h1;
            return h0;
        }

        edge_type* make_edge(hege_type* hege)
        {
            auto* e = edges.alloc();
            e->any_hege = hege;
            hege->edge = e;
            hege->twin->edge = e;
            return e;
        }

        bool close_hege(hege_type* h0, hege_type* h1)
        {
            if (h0->twin->start != h1->start) return false;
            h0->next = h1;
            h1->prev = h0;
            h1->twin->next = h0->twin;
            h0->twin->prev = h1->twin;
            return true;
        }

    private:
        // Expands to unbound_slab<body_type> bodys; and etc.
        #define STRUCT(NAME) unbound_slab<NAME>
        #define END_STRUCT(VAR) VAR ## s;
        #include "primitive.inl"

        body_type* any_body{};
    };
}

