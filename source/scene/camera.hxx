#pragma once
#include "../lib/glm/vec3.hxx"
#include "../lib/glm/fwd.hxx"
#include "../lib/mapbox/variant.hxx"
#include "../math/direction.hxx"
#include "../math/turn-table.hxx"

namespace hegem::scene
{
    namespace cameras
    {
        using math::direction_type;

        struct pin_hole
        {
            glm::vec3 center;
            direction_type forward;
            direction_type up;
            float fov;
            math::turn_table tt{};
        };

        struct orthographic
        {
            glm::vec3 center;
            direction_type forward;
            direction_type up;
            float size;
            math::turn_table tt{};
        };

        using camera_type = mapbox::util::variant<orthographic, pin_hole>;

        glm::mat4 camera_space_to_world_space(camera_type const& cam);
        glm::mat4 world_space_to_camera_space(camera_type const& cam);
        glm::mat4 projection_of(camera_type const& cam, float aspect_ratio);
        glm::mat4 world_space_to_clip_space(camera_type const& cam, float aspect_ratio);
        glm::vec3 apex_in_world_space(camera_type const& cam, float aspect_ratio);
    }

    using cameras::camera_type;
    using cameras::camera_space_to_world_space;
    using cameras::world_space_to_clip_space;
    using cameras::apex_in_world_space;
}

