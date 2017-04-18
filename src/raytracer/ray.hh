#pragma once
#include "../lib/glm/vec2.hh"
#include "../lib/glm/vec3.hh"
#include "../math/unit.hh"
#include "../scene/camera.hh"

namespace rt::raytracer
{
    namespace ray_details
    {
        using position_type = glm::vec2;
        using scene::camera_type;

        struct ray_type
        {
            using point_type = glm::vec3;
            using direction_type = math::unit<glm::vec3>;

            point_type origin;
            direction_type dir;

            point_type at(float extent) const { return origin + (*dir) * extent; }
        };

        ray_type camera_ray_from_camera_plane(position_type const& p, camera_type const& cam);
    }

    using ray_details::ray_type;
    using ray_details::camera_ray_from_camera_plane;
}

