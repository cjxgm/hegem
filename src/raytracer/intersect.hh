#pragma once
#include "../scene/node.hh"
#include "hit.hh"
#include "ray.hh"

namespace rt::raytracer
{
    object_hit_type intersect(scene::node_type const& node, ray_type const& ray);
}

