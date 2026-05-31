#include "counter.hh"

namespace rt::global::counter_details
{
    aggregation_type counter;

    void aggregation_type::reset()
    {
        pixel = 0;
        ray = 0;
        ray_refl = 0;
        ray_refr = 0;
        ray_shadow = 0;
        ray_bvh_incoming = 0;
        ix = 0;
        ix_bvh = 0;
        ix_bvh_bound = 0;
        ix_bvh_face = 0;
        ix_grid = 0;
        ix_grid_face = 0;
    }
}

