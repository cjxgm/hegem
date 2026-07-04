#include "counter.hxx"

namespace hegem::global::counter_details
{
    aggregation_type counter;

    auto aggregation_type::reset() -> void
    {
        pixel.store(0, std::memory_order::relaxed);
        ray.store(0, std::memory_order::relaxed);
        ray_refl.store(0, std::memory_order::relaxed);
        ray_refr.store(0, std::memory_order::relaxed);
        ray_shadow.store(0, std::memory_order::relaxed);
        ray_bvh_incoming.store(0, std::memory_order::relaxed);
        ix.store(0, std::memory_order::relaxed);
        ix_bvh.store(0, std::memory_order::relaxed);
        ix_bvh_bound.store(0, std::memory_order::relaxed);
        ix_bvh_face.store(0, std::memory_order::relaxed);
        ix_grid.store(0, std::memory_order::relaxed);
        ix_grid_face.store(0, std::memory_order::relaxed);
        task_started.store(0, std::memory_order::relaxed);
        task_stopped.store(0, std::memory_order::relaxed);
    }
}

