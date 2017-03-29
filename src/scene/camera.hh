#pragma once
#include "../lib/mapbox/variant.hh"
#include "../math/unit.hh"

#define WANT_VEC3
#define WANT_MAT3
#define WANT_MAT4
#include "../lib/glm.hh"
#undef WANT_MAT4
#undef WANT_MAT3
#undef WANT_VEC3

namespace rt::scene
{
    namespace cameras
    {
        using direction_type = math::unit<glm::vec3>;

        struct pin_hole
        {
            // TODO
            glm::mat4 transformation() const { return {}; }
            glm::mat3 rotation() const { return {}; }
        };

        struct orthographic
        {
            orthographic(
                    glm::vec3 center,
                    direction_type forward,
                    direction_type up,
                    float size)
                : center{center}
                , forward{forward}
                , up{up}
                , size{size}
            {}

            glm::mat4 transformation() const;
            glm::mat3 rotation() const;

        private:
            glm::vec3 center;
            direction_type forward;
            direction_type up;
            float size;
        };

        using camera_type = mapbox::util::variant<orthographic, pin_hole>;
    }

    using cameras::camera_type;
}

