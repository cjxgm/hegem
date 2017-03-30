#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/mapbox/variant.hh"
#include "../scene/material.hh"
#include "../math/unit.hh"
#include "ray.hh"

namespace rt::raytracer
{
    namespace hits
    {
        using point_type = glm::vec3;
        using direction_type = math::unit<glm::vec3>;
        using scene::material_id_type;

        struct geometry
        {
            ray_type ray;
            float ray_extent;
            point_type hit_point;
            direction_type normal;
        };

        struct object
        {
            material_id_type material_id;
            geometry geom;
        };

        struct missed
        {
            ray_type ray;
        };

        using geometry_hit_type = mapbox::util::variant<
            geometry,
            missed
        >;

        using object_hit_type = mapbox::util::variant<
            object,
            missed
        >;

        // missed < missed: false
        // missed < hit   : false
        //    hit < missed: true
        //    hit < hit   : (a.extent < b.extent)
        bool less_extent(object_hit_type const& a, object_hit_type const& b);
        object_hit_type extent_lesser_one(object_hit_type a, object_hit_type b);
    }

    using hits::geometry_hit_type;
    using hits::object_hit_type;
}

