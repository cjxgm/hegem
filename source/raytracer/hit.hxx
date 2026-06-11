#pragma once
#include "../lib/glm/vec3.hxx"
#include "../lib/mapbox/variant.hxx"
#include "../scene/material.hxx"
#include "../math/direction.hxx"
#include "../image/image.hxx"
#include "../image/color.hxx"
#include "ray.hxx"

namespace hegem::raytracer
{
    namespace hits
    {
        using point_type = glm::vec3;
        using math::direction_type;
        using color_type = image::color::linear_rgb;
        using scene::material_id_type;

        struct shape
        {
            ray_type viewing;
            float ray_extent;
            float ray_max_error;
            point_type hit_point;
            direction_type normal;
        };

        struct object
        {
            material_id_type material_id;
            shape shape_info;
        };

        struct missed
        {
            ray_type viewing;
        };

        using shape_hit_type = mapbox::util::variant<
            shape,
            missed
        >;

        using object_hit_type = mapbox::util::variant<
            object,
            missed
        >;

        struct shaded_object_hit_type
        {
            object_hit_type hit;
            color_type radiance;
        };

        // returns a.viewing
        ray_type viewing_ray(object_hit_type const& a);
        ray_type viewing_ray(shape_hit_type const& a);
        // returns +inf if missed
        // returns a.ray_extent otherwise
        float ray_extent(object_hit_type const& a);
        float ray_extent(shape_hit_type const& a);
        // returns ray_extent(a) < ray_extent(b)
        bool less_extent(object_hit_type const& a, object_hit_type const& b);
        bool less_extent(shape_hit_type const& a, shape_hit_type const& b);
        // returns a if less_extent(a, b)
        // returns b otherwise
        object_hit_type extent_lesser_one(object_hit_type a, object_hit_type b);
        shape_hit_type extent_lesser_one(shape_hit_type a, shape_hit_type b);

        ray_type biased_ray(ray_type ray, shape shape_info);

        using hit_buffer_type = image::image<object_hit_type>;
    }

    using hits::shape_hit_type;
    using hits::object_hit_type;
    using hits::shaded_object_hit_type;
    using hits::hit_buffer_type;
}

namespace hegem::image::image_impl
{
    extern template struct image<raytracer::object_hit_type>;
}

