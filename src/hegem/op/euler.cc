#include "../hemesh.hh"
#include "../primitive.hh"
#include "../list.hh"
#include "euler.hh"
#include <stdexcept>

namespace rt::hegem
{
    inline namespace op
    {
        inline namespace euler
        {
            body_type* make_body(hemesh& m, position_type pos)
            {
                auto b = m.make_body();
                auto f = m.make_face(b);
                auto v = m.make_vert();
                m.make_ring(f, v);
                v->pos = pos;
                return b;
            }

            edge_type* make_edge(hemesh& m, ring_type* ring, position_type pos)
            {
                auto v = m.make_vert();
                auto h = m.make_hege_twin(ring, v);
                auto e = m.make_edge(h);
                v->pos = pos;
                return e;
            }

            edge_type* make_edge(hemesh& m, hege_type* hege, position_type pos)
            {
                auto v = m.make_vert();
                auto h = m.make_hege_twin(hege, v);
                auto e = m.make_edge(h);
                v->pos = pos;
                return e;
            }

            face_type* make_face(hemesh& m, hege_type* first, hege_type* last)
            {
                auto h = m.make_hege_twin(last, first->start);
                m.make_edge(h);
                m.close_hege(h, first);
                first->ring->any_hege = h->twin;
                first->ring->any_vert = h->twin->start;

                auto b = first->ring->face->body;
                auto f = m.make_face(b);
                auto r = m.make_ring(f, first->start);

                r->any_hege = first;
                for (auto& h: list::iterate(first))
                    h.ring = r;

                return f;
            }

            ring_type* kill_bridge(hemesh& m, hege_type* bridge_to_inner)
            {
                if (bridge_to_inner->next->twin == bridge_to_inner) {
                    throw std::invalid_argument{
                        "Expecting a bridge, but the provided hege "
                        "connects to a single vertex."
                    };
                }

                if (bridge_to_inner->prev->twin == bridge_to_inner) {
                    throw std::invalid_argument{
                        "Expecting a bridge, but the provided hege "
                        "connects from a single vertex."
                    };
                }

                auto h0 = bridge_to_inner;
                auto h1 = h0->twin;

                h0->start->any_hege = h0->prev->twin;
                h1->start->any_hege = h1->prev->twin;

                auto outer_ring = h0->ring;
                outer_ring->any_hege = h0->prev;
                outer_ring->any_vert = h0->start;

                auto inner_ring = m.make_ring(outer_ring->face, h0->next->start);
                inner_ring->any_hege = h0->next;

                list::connect(h0->prev, h1->next);
                list::connect(h1->prev, h0->next);

                m.free(h0->edge);
                m.free(h0);
                m.free(h1);

                for (auto& h: list::iterate(inner_ring->any_hege))
                    h.ring = inner_ring;

                return inner_ring;
            }

            ring_type* face_to_ring(hemesh& m, face_type* inner_face, face_type* outer_face)
            {
                auto ring = inner_face->boundary;

                if (ring->next != ring) {
                    throw std::invalid_argument{
                        "Inner face cannot have any inner ring."
                    };
                }

                ring->face = outer_face;
                list::append(outer_face->boundary, ring);

                list::remove(inner_face);
                m.free(inner_face);

                return ring;
            }

            edge_type* split_edge(hemesh& m, edge_type* edge, position_type pos)
            {
                auto ha0 = edge->any_hege;
                auto ha1 = ha0->twin;
                auto hb0 = list::insert_after(ha1, m.heges.alloc());
                auto hb1 = list::insert_after(ha0, m.heges.alloc());

                auto v = m.make_vert();
                v->pos = pos;
                v->any_hege = hb1;

                hb0->start = v;
                hb1->start = v;

                ha0->twin = hb0;
                hb0->twin = ha0;
                ha1->twin = hb1;
                hb1->twin = ha1;

                hb0->ring = ha1->ring;
                hb1->ring = ha0->ring;

                hb0->edge = ha0->edge;
                ha0->edge->any_hege = ha0;
                return m.make_edge(hb1);
            }
        }

        inline namespace euler_reverse
        {
            hege_type* bridge(hemesh& m, hege_type* p0, hege_type* p1)
            {
                if (p0->ring == p1->ring) {
                    throw std::invalid_argument{
                        "Bridging connects 2 rings, but the 2 heges is on the same ring."
                    };
                }

                if (p0->ring->face != p1->ring->face) {
                    throw std::invalid_argument{
                        "The 2 rings to bridge must be in the same face."
                    };
                }

                auto ring = p0->ring;
                if (ring->face->boundary == p1->ring)
                    ring->face->boundary = ring;
                list::remove(p1->ring);
                m.free(p1->ring);

                for (auto& h: list::iterate(p1))
                    h.ring = ring;

                auto h = m.make_hege_twin(p0->prev, p1->start);
                m.close_hege(h, p1);
                m.make_edge(h);
                return h;
            }
        }
    }
}

