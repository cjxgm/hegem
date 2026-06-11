#pragma once
#include "../scene/cache.hxx"
#include "hit.hxx"
#include "ray.hxx"

namespace hegem::raytracer
{
    object_hit_type intersect(scene::scene_cache const& sc, ray_type const& ray);

    // Return true if `ray` intersected `node` within `extent`
    // Return false if `ray` didn't intersect `node` or the intersection is outside `extent`
    bool is_intersected_within(scene::scene_cache const& sc, ray_type const& ray, float extent);
}

