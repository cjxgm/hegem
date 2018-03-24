#pragma once
#include "../lib/std/optional.hh"
#include "../image/color.hh"
#include "../scene/scene.hh"
#include "../math/sampler.hh"
#include "../raytracer/ray.hh"
#include "../raytracer/hit.hh"

namespace rt::pathtracer
{
    namespace shading_details
    {
        using color_type = image::color::linear_rgb;
        using scene::scene_type;
        using raytracer::ray_type;
        using raytracer::object_hit_type;

        struct shading_point
        {
            lib::optional<ray_type> next_ray;
            color_type ray_color;   // already have 1 / probability built in.
            color_type emission;

            // Holdout. Stop further raycasting.
            shading_point(): shading_point{color_type{}} {}

            // Emission-only. Stop further raycasting.
            shading_point(color_type emission)
                : next_ray{}
                , ray_color{}
                , emission{emission}
            {}

            shading_point(ray_type ray, color_type color, float probability, color_type emission)
                : next_ray{ray}
                , ray_color{color / probability}
                , emission{emission}
            {}
        };

        auto shade(scene_type const& scene, object_hit_type const& hit, math::normal_sampler & samp) -> shading_point;
    }

    using shading_details::shading_point;
    using shading_details::shade;
}

