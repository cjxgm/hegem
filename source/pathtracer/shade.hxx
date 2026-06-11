#pragma once
#include "../lib/std/optional.hxx"
#include "../math/sampler.hxx"
#include "../image/color.hxx"
#include "../scene/scene.hxx"
#include "../raytracer/ray.hxx"
#include "../raytracer/hit.hxx"

namespace hegem::pathtracer
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

            shading_point(ray_type ray, color_type color, float weight, color_type emission)
                : next_ray{ray}
                , ray_color{color * (
                    weight > 4.0f ? 4.0f :      // clamp fireflies
                    weight != weight ? 0.0f :   // NaN (black dots)
                    weight
                )}
                , emission{emission}
            {}
        };

        auto shade(
            scene_type const& scene,
            object_hit_type const& hit,
            math::uniform_sampler& canonical_sampler
        ) -> shading_point;
    }

    using shading_details::shading_point;
    using shading_details::shade;
}

