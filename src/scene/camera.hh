#pragma once
#include "../lib/mapbox/variant.hh"

#define WANT_VEC3
#define WANT_MAT4
#include "../lib/glm.hh"
#undef WANT_MAT4
#undef WANT_VEC3

namespace rt::scene
{
    namespace cameras
    {
        struct pin_hole
        {
            // TODO
        };

        struct orthographic
        {
            orthographic(
                    glm::vec3 center,
                    glm::vec3 direction,
                    glm::vec3 up,
                    float size)
                : center{center}
                , direction{direction}
                , up{up}
                , size{size}
            {}

            glm::mat4 transformation() const;

        private:
            glm::vec3 center;
            glm::vec3 direction;
            glm::vec3 up;
            float size;
        };

        using camera_type = mapbox::util::variant<orthographic, pin_hole>;
    }

    using cameras::camera_type;
}

