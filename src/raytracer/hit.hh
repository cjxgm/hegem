#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/mapbox/variant.hh"
#include "../scene/material.hh"
#include "../math/unit.hh"
#include "../image/image.hh"
#include "../image/color.hh"
#include "ray.hh"

namespace rt::raytracer
{
    namespace hits
    {
        using point_type = glm::vec3;
        using direction_type = math::unit<glm::vec3>;
        using color_type = image::color::linear_rgb;
        using scene::material_id_type;

        struct shape
        {
            ray_type ray;
            float ray_extent;
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
            ray_type ray;
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

        // returns +inf if missed
        // returns a.ray_extent otherwise
        float ray_extent(object_hit_type const& a);
        // returns ray_extent(a) < ray_extent(b)
        bool less_extent(object_hit_type const& a, object_hit_type const& b);
        object_hit_type extent_lesser_one(object_hit_type a, object_hit_type b);

        using hit_buffer_type = image::image<object_hit_type>;
    }

    using hits::shape_hit_type;
    using hits::object_hit_type;
    using hits::shaded_object_hit_type;
    using hits::hit_buffer_type;
}

namespace rt::image::image_impl
{
    extern template struct image<raytracer::object_hit_type>;
}

