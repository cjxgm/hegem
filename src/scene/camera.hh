#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/glm/fwd.hh"
#include "../lib/mapbox/variant.hh"
#include "../math/unit.hh"

namespace rt::scene
{
    namespace cameras
    {
        using direction_type = math::unit<glm::vec3>;

        struct pin_hole
        {
            glm::vec3 center;
            direction_type forward;
            direction_type up;
            float fov;
        };

        struct orthographic
        {
            glm::vec3 center;
            direction_type forward;
            direction_type up;
            float size;
        };

        using camera_type = mapbox::util::variant<orthographic, pin_hole>;

        glm::mat3 camera_space_to_world_space_rotation_only(camera_type const& cam);
        glm::mat4 camera_space_to_world_space(camera_type const& cam);
    }

    using cameras::camera_type;
    using cameras::camera_space_to_world_space_rotation_only;
    using cameras::camera_space_to_world_space;
}

