#pragma once
#include "../lib/glm/vec2.hh"
#include "../lib/glm/fwd.hh"
#include "camera.hh"
#include <utility>

namespace rt::scene
{
    struct view_type
    {
        using dimension_type = glm::ivec2;

        dimension_type size;
        camera_type camera;

        glm::mat3 screen_space_to_camera_plane_space() const;
    };
}

