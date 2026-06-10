#pragma once
#include "../lib/glm/vec2.hxx"
#include "../lib/glm/vec3.hxx"
#include "../math/direction.hxx"
#include "../scene/camera.hxx"

namespace rt::raytracer
{
    namespace ray_details
    {
        using position_type = glm::vec2;
        using math::direction_type;
        using scene::camera_type;

        struct ray_type
        {
            using point_type = glm::vec3;

            point_type origin;
            direction_type dir;

            point_type at(float extent) const { return origin + (*dir) * extent; }
        };

        ray_type camera_ray_from_camera_plane(position_type const& p, camera_type const& cam);
    }

    using ray_details::ray_type;
    using ray_details::camera_ray_from_camera_plane;
}

