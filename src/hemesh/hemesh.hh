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
            return list_append(first_body, bodys.alloc());
        }

        auto* make_face(body_type* body)
        {
            auto* f = list_append(body->first_face, faces.alloc());
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
            r->first_vert = vert;
            return r;
        }

        hege_type* make_hege_twin(ring_type* ring, vert_type* vert)
        {
            if (ring->first_hege) return nullptr;

            auto* h0 = list_append(ring->first_hege, heges.alloc());
            auto* h1 = list_append(ring->first_hege, heges.alloc());
            h0->twin = h1;
            h1->twin = h0;
            h0->ring = ring;
            h1->ring = ring;
            h0->start = ring->first_vert;
            h1->start = vert;
            h0->start->first_hege = h0;
            h1->start->first_hege = h1;
            return h0;
        }

        hege_type* make_hege_twin(hege_type* hege, vert_type* vert)
        {
            if (!hege->ring->first_hege) return nullptr;

            auto* h0 = list_insert_after(hege, heges.alloc());
            auto* h1 = list_insert_after(h0,   heges.alloc());
            h0->twin = h1;
            h1->twin = h0;
            h0->ring = hege->ring;
            h1->ring = hege->ring;
            h0->start = hege->twin->start;
            h1->start = vert;
            h0->start->first_hege = h0;
            h1->start->first_hege = h1;
            return h0;
        }

        edge_type* make_edge(hege_type* hege)
        {
            auto* e = edges.alloc();
            e->first_hege = hege;
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
        unbound_slab<primitive::body_type> bodys;
        unbound_slab<primitive::face_type> faces;
        unbound_slab<primitive::ring_type> rings;
        unbound_slab<primitive::edge_type> edges;
        unbound_slab<primitive::hege_type> heges;
        unbound_slab<primitive::vert_type> verts;
        body_type* first_body{};

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

