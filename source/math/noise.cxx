#include "../lib/glm/vec2.hxx"
#include "../lib/glm/vec3.hxx"
#include "noise.hxx"

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

namespace rt::math
{
    inline namespace noise
    {
        auto fbm_noise(glm::vec3 pos, int details, float size, glm::vec2 range, glm::vec3 seed) -> float
        {
            pos /= size;
            pos += seed;
            auto x = stb_perlin_fbm_noise3(pos.x, pos.y, pos.z, 2.0f, 1.0f, details, 0, 0, 0);
            if (range.y < range.x) range.y = range.x;
            if (x < range.x) return 0.0f;
            if (x >= range.y) return 1.0f;
            return (x - range.x) / (range.y - range.x);
        }
    }
}

