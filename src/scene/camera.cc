#include "../lib/glm/vec4.hh"
#include "../lib/glm/op/geom.hh"
#include "camera.hh"

namespace rt::scene::cameras
{
    glm::mat4 orthographic::transformation() const
    {
        auto half = size / 2.0f;
        auto rot = rotation();
        return {
            glm::vec4{rot[0] * half, 0},
            glm::vec4{rot[1] * half, 0},
            glm::vec4{rot[2]       , 0},
            glm::vec4{center       , 1},
        };
    }

    glm::mat3 orthographic::rotation() const
    {
        direction_type right = cross(*forward, *up);
        direction_type proper_up = cross(*right, *forward);
        return {
            *right,
            *proper_up,
            -*forward, // <--- This. It will help you understand your question in src/raytracer/shade.cc.
        };
    }
}

