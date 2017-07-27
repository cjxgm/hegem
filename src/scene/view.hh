#pragma once
#include "../lib/glm/vec2.hh"
#include "../lib/glm/fwd.hh"
#include "camera.hh"
#include <utility>
#include <string>

namespace rt::scene
{
    struct view_type
    {
        using dimension_type = glm::ivec2;

        std::string name;
        dimension_type size;
        int bounces;
        int samples;
        camera_type camera;

        glm::mat3 screen_space_to_camera_plane_space() const;
    };
}

