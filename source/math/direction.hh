#pragma once
#include "../lib/glm/vec3.hh"
#include "unit.hh"

namespace rt::math
{
    using direction_type = unit<glm::vec3>;

    inline auto operator * (direction_type a, float b) { return *a * b; }
    inline auto operator * (float a, direction_type b) { return a * *b; }
}

