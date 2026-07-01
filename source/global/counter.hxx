#pragma once
#include <atomic>

namespace hegem::global
{
    namespace counter_details
    {
        using counter_type = std::atomic_long;

        struct aggregation_type
        {
            counter_type pixel;                 // Number of pixels raytraced
            counter_type ray;                   // Number of rays produced for raytracing
            counter_type ray_refl;              // Number of reflection rays produced
            counter_type ray_refr;              // Number of refraction rays produced
            counter_type ray_shadow;            // Number of shadow rays produced
            counter_type ray_bvh_incoming;      // Number of rays coming to BVH for intersection tests
            counter_type ix;                    // Number of intersection tests performed
            counter_type ix_bvh;                // Number of intersection tests performed on BVH
            counter_type ix_bvh_bound;          // Number of intersection tests performed on BVH internal nodes bounds
            counter_type ix_bvh_face;           // Number of intersection tests against faces performed on BVH leaf nodes
            counter_type ix_grid;               // Number of intersection tests performed on grid
            counter_type ix_grid_face;          // Number of intersection tests against faces performed on grid
            counter_type task_started;          // Number of tasks started
            counter_type task_stopped;          // Number of tasks stopped, no matter due to success or failure

            auto reset() -> void;
        };

        extern aggregation_type counter;
    }

    using counter_details::counter;
}

