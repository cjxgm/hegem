#pragma once
#include "primitive.hh"
#include "slab.hh"

namespace rt::hemesh
{
    struct hemesh
    {
        // makers.
        //
        // All makers establish topological connections.
        auto* make_body()
        {
            return list_append(any_body, bodys.alloc());
        }

        auto* make_face(body_type* body)
        {
            auto* f = list_append(body->any_face, faces.alloc());
            f->body = body;
            return f;
        }

        auto* make_vert()
        {
            return verts.alloc();
        }

        auto* make_ring(face_type* face, vert_type* vert)
        {
            auto* r = list_append(face->boundary, rings.alloc());
            r->face = face;
            r->any_vert = vert;
            return r;
        }

        hege_type* make_hege_twin(ring_type* ring, vert_type* vert)
        {
            if (ring->any_hege) return nullptr;

            auto* h0 = list_append(ring->any_hege, heges.alloc());
            auto* h1 = list_append(ring->any_hege, heges.alloc());
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

            auto* h0 = list_insert_after(hege, heges.alloc());
            auto* h1 = list_insert_after(h0,   heges.alloc());
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

        template <class T>
        static T* list_append(T*& a, T* x)
        {
            if (!a) a = x;
            return list_insert_before(a, x);
        }

        template <class T>
        static T* list_insert_after(T* pivot, T* x)
        {
            x->next = pivot->next;
            x->prev = pivot;
            x->prev->next = x;
            x->next->prev = x;
            return x;
        }

        template <class T>
        static T* list_insert_before(T* pivot, T* x)
        {
            x->next = pivot;
            x->prev = pivot->prev;
            x->next->prev = x;
            x->prev->next = x;
            return x;
        }
    };
}

