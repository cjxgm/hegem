#pragma once
#include "../lib/glm/vec2.hh"
#include "../lib/glm/mat3.hh"
#include "camera.hh"
#include <utility>

namespace rt::scene
{
    struct view_type
    {
        using dimension_type = glm::ivec2;

        view_type(dimension_type size, camera_type camera)
            : size_{std::move(size)}
            , camera_{std::move(camera)}
        {}

        glm::mat3 screen_space_to_camera_plane_space() const;

        auto& size() const { return size_; }
        auto& camera() const { return camera_; }

    private:
        dimension_type size_;
        camera_type camera_;
    };
}

