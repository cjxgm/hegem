#pragma once
// fsck: Formal Self-ChecK, or, "i want to F*CK my past Self".
#include "hemesh.fwd.hxx"

namespace rt::swing
{
    // fsck::fsck_* functions return true when bad things happened.
    inline namespace fsck
    {
        // do all the following fscks
        bool fsck_all(hemesh const& m);

        // check missing slab.free(x) calls
        // and whether a pointer points to an illegal address
        bool fsck_pointers(hemesh const& m);

        // check if, e.g., ring->any_hege->ring == ring
        bool fsck_link_consistency(hemesh const& m);
    }
}

