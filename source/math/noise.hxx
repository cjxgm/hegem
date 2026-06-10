#pragma once
#include "../lib/glm/fwd.hxx"

namespace rt::math
{
    inline namespace noise
    {
        auto fbm_noise(glm::vec3 pos, int details, float size, glm::vec2 range, glm::vec3 seed={}) -> float;
    }
}

