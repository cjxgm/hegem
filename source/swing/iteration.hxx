#pragma once
// Extra iteration utilities for iterating along the topology
#include "list.hxx"
#include "primitive.hxx"

namespace hegem::swing
{
    namespace iter
    {
        // all outgoing heges around the start vertex of a hege
        inline auto heges_around_vert(hege_type* first, hege_type* start=nullptr)
        {
            struct next_hege
            {
                using value_type = hege_type;
                static value_type* next(value_type* h) { return h->prev->twin; }
            };
            return list::range<next_hege>(first, start);
        }

        //        │                           │     │     │     │     │
        //        │                           │     │     │     │     │
        //      ──┴──                   ··· ══╧═════╧═════╧═════╧═════╧══ ···
        // The above is a T-joint. Multiple T-joint forms a T-ring (the double-lined part).
        //
        //
        //
        //           │     │
        //           │     │
        //  ··· ──>──┴──>──┴──>── ···
        //        │     ╰╴next
        //        ╰──────╴first
        //
        inline auto heges_on_T_ring(hege_type* first, hege_type* start=nullptr)
        {
            struct next_on_T_ring
            {
                using value_type = hege_type;
                static value_type* next(value_type* h) { return h->next->twin->next; }
            };
            return list::range<next_on_T_ring>(first, start);
        }
    }
}

