#include "../../lib/glm/op/geom.hh"
#include "../hemesh.hh"
#include "../primitive.hh"
#include "../geometry.hh"
#include "sweep.hh"
#include "euler.hh"
#include <stdexcept>

namespace rt::hegem
{
    inline namespace op
    {
        inline namespace sweep
        {
            void extrude(hemesh & m, face_type* face, offset_type offset, float eps)
            {
                auto ring = face->boundary;
                if (dot(offset, offset) <= eps*eps) {
                    throw std::invalid_argument{
                        "Extrusion offset too small."
                    };
                }

                auto n = normal(ring->any_hege, eps);
                if (!is_same_side(n, offset, eps)) {
                    throw std::invalid_argument{
                        "Cannot extrude inward."
                    };
                }

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

