#pragma once
#include "hit.hh"
#include "../image/image.hh"
#include "../scene/scene.hh"
#include <tuple>

namespace rt::raytracer
{
    namespace raytracer_details
    {
        using image_type = image::image_rgb;
        using raytracing_result_type = std::tuple<image_type, hit_buffer_type>;
        using scene::scene_type;

        shaded_object_hit_type raytrace(scene_type const& scene, ray_type const& ray, int remaining_bounce_count);
        raytracing_result_type raytrace(scene_type const& scene, int view_id, int max_bounce_count);
    }

    using raytracer_details::raytrace;
    using raytracer_details::raytracing_result_type;
}

