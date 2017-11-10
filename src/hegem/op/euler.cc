#include "../hemesh.hh"
#include "../primitive.hh"
#include "euler.hh"

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
                auto r = m.make_ring(f, v);
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

                auto b = first->ring->face->body;
                auto f = m.make_face(b);
                auto r = m.make_ring(f, first->start);

                h->ring->any_hege = h->twin;
                for (h=first; true; h=h->next) {
                    h->ring = r;
                    if (h->next == first) break;
                }
                r->any_hege = first;

                return f;
            }
        }
    }
}

