#pragma once
#include "path.hh"
#include "../scene/scene.hh"

namespace rt::raytracer
{
    path_type raytrace(scene::scene_type const& scene, ray_type const& ray, int remaining_bounce_count);
    path_buffer_type raytrace(scene::scene_type const& scene, int view_id, int max_bounce_count);
}

