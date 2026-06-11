#include "../../lib/glm/op/geom.hxx"
#include "../hemesh.hxx"
#include "../primitive.hxx"
#include "../geometry.hxx"
#include "../list.hxx"
#include "../iteration.hxx"
#include "sweep.hxx"
#include "euler.hxx"
#include <stdexcept>

namespace hegem::swing
{
    inline namespace op
    {
        inline namespace sweep
        {
            namespace
            {
                void extrude_ring(hemesh & m, ring_type* ring, offset_type offset)
                {
                    auto first = ring->any_hege;

                    for (auto& h: list::iterate(first))
                        make_edge(m, &h, h.next->start->pos + offset);

                    for (auto& h: iter::heges_on_T_ring(first))
                        make_face(m, h.prev, h.next);
                }
            }

            void extrude(hemesh & m, face_type* face, offset_type offset)
            {
                for (auto& r: list::iterate(face->boundary))
                    extrude_ring(m, &r, offset);
            }
        }
    }
}

