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
            : size{std::move(size)}
            , camera{std::move(camera)}
        {}

        glm::mat4 transformation() const;

        const dimension_type size;

    private:
        camera_type camera;
    };
}

