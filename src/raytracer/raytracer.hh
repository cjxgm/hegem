#pragma once
#include "../lib/glm/fwd.hh"
#include "../image/image.hh"
#include "../scene/scene.hh"
#include "../scene/view.hh"
#include "../util/tile.hh"
#include "hit.hh"
#include "ray.hh"
#include <tuple>

namespace rt::raytracer
{
    namespace raytracer_details
    {
        using image_type = image::image_rgb;
        using raytracing_result_type = std::tuple<image_type, hit_buffer_type>;
        using scene::scene_type;
        using scene::view_type;

        struct ray_extent_type
        {
            ray_type ray;
            float extent;

            ray_extent_type(ray_type ray, float extent)
                : ray{ray}, extent{extent} {}
        };
        using ray_extent_list = std::vector<ray_extent_type>;

        raytracing_result_type raytrace(scene_type const& scene, view_type const& view, util::tile const& tile);
        ray_extent_list raytrace(scene_type const& scene, view_type const& view, glm::vec2 screen_pos);
    }

    using raytracer_details::raytrace;
    using raytracer_details::raytracing_result_type;
    using raytracer_details::ray_extent_type;
    using raytracer_details::ray_extent_list;
}

