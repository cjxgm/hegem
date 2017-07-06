#pragma once
#include "../scene/node.hh"
#include "hit.hh"
#include "ray.hh"

namespace rt::raytracer
{
    object_hit_type intersect(scene::node_type const& node, ray_type const& ray);

    // Return true if `ray` intersected `node` within `extent`
    // Return false if `ray` didn't intersect `node` or the intersection is outside `extent`
    bool is_intersected_within(scene::node_type const& node, ray_type const& ray, float extent);
}

