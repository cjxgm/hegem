#include "hemesh.hxx"
#include "list.hxx"
#include "meta.hxx"
#include "dump.hxx"
#include "fsck.hxx"
#include <stdexcept>
#include <string>

// misc
namespace hegem::swing
{
    void hemesh::extend(hemesh const& m)
    {
        meta::extend(m, *this);
    }

    void hemesh::diagnose(char const* situation) const
    {
        dump_memory_usage_map(*this);
        if (fsck_all(*this)) {
            throw std::logic_error{
                std::string{} + "Failed fsck " + situation + ". "
                "There should be a bug somewhere."
            };
        }
    }
}

// makers
namespace hegem::swing
{
    body_type* hemesh::make_body()
    {
        return list::append(any_body, bodys.alloc());
    }

    face_type* hemesh::make_face(body_type* body)
    {
        auto* f = list::append(body->any_face, faces.alloc());
        f->body = body;
        return f;
    }

    vert_type* hemesh::make_vert()
    {
        return verts.alloc();
    }

    ring_type* hemesh::make_ring(face_type* face, vert_type* vert)
    {
        auto* r = list::append(face->boundary, rings.alloc());
        r->face = face;
        r->any_vert = vert;
        return r;
    }

    hege_type* hemesh::make_hege_twin(ring_type* ring, vert_type* vert)
    {
        if (ring->any_hege) {
            throw std::logic_error{
                "Cannot make heges on a ring that already has an hege"
            };
        }

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

    hege_type* hemesh::make_hege_twin(hege_type* hege, vert_type* vert)
    {
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

    edge_type* hemesh::make_edge(hege_type* hege)
    {
        auto* e = edges.alloc();
        e->any_hege = hege;
        hege->edge = e;
        hege->twin->edge = e;
        return e;
    }

    //        h0
    //    *<-----
    //   /
    //  / h1
    // v
    //
    // assert: start(h1) == end(h0)
    void hemesh::close_hege(hege_type* h0, hege_type* h1)
    {
        if (h0->twin->start != h1->start) {
            throw std::logic_error{
                "Can only close consecutive heges that share an intermediate point."
            };
        }

        list::connect(h1->prev, h0->twin);
        list::connect(h0, h1);
    }
}

