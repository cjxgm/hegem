#pragma once
#include "camera.hh"
#include <utility>

#define WANT_VEC2
#define WANT_MAT4
#include "../lib/glm.hh"
#undef WANT_MAT4
#undef WANT_VEC2

namespace rt::scene
{
    struct view_type
    {
        using dimension_type = glm::ivec2;

        view_type(dimension_type size, camera_type camera)
            : size_{std::move(size)}
            , camera_{std::move(camera)}
        {}

        glm::mat4 transformation() const;
        glm::mat3 camera_rotation() const;

        auto& size() const { return size_; }
        auto& camera() const { return camera_; }

    private:
        dimension_type size_;
        camera_type camera_;
    };
}

