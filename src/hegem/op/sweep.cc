#include "../../lib/glm/op/geom.hh"
#include "../hemesh.hh"
#include "../primitive.hh"
#include "../geometry.hh"
#include "sweep.hh"
#include "euler.hh"

namespace rt::hegem
{
    inline namespace op
    {
        inline namespace sweep
        {
            void extrude(hemesh & m, ring_type* ring, offset_type offset, float eps)
            {
                if (ring == nullptr) return;
                if (ring->any_hege == nullptr) return;
                if (dot(offset, offset) <= eps*eps) return;

                auto n = normal(ring->any_hege, eps);
                if (!is_same_side(n, offset, eps)) return;

                auto first = ring->any_hege;
                for (auto now=first;;) {
                    auto next = now->next;
                    make_edge(m, now, next->start->pos + offset);
                    if ((now = next) == first) break;
                }

                for (auto now=first;;) {
                    make_face(m, now->prev, now->next);
                    if ((now = now->next->twin->next) == first) break;
                }
            }
        }
    }
}

