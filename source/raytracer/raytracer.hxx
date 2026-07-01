#pragma once
#include "../lib/glm/fwd.hxx"
#include "../image/image.hxx"
#include "../image/color.hxx"
#include "../scene/scene.hxx"
#include "../scene/view.hxx"
#include "../tool/tile.hxx"
#include "hit.hxx"
#include "ray.hxx"
#include <tuple>
#include <memory>
#include <atomic>

namespace hegem::raytracer
{
    namespace raytracer_details
    {
        using image_type = image::image_rgb;
        using color_type = image::color::linear_rgb;
        using raytracing_result_type = std::tuple<image_type, hit_buffer_type>;
        using shared_canceled_type = std::shared_ptr<std::atomic_bool>;
        using scene::scene_type;
        using scene::view_type;

        struct ray_visualization
        {
            ray_type ray;
            float extent;
            color_type color;
            float width;

            ray_visualization(ray_type ray, float extent, color_type color, float width)
                : ray{ray}, extent{extent}, color{color}, width{width} {}
        };
        using ray_visualizations = std::vector<ray_visualization>;

        auto raytrace(shared_canceled_type shared_canceled, scene_type const& scene, view_type const& view, tool::tile const& tile) -> raytracing_result_type;
        auto raytrace(scene_type const& scene, view_type const& view, glm::vec2 screen_pos) -> ray_visualizations;
    }

    using raytracer_details::raytrace;
    using raytracer_details::raytracing_result_type;
    using raytracer_details::ray_visualization;
    using raytracer_details::ray_visualizations;
}

