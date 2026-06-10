#pragma once
// basic types
#include "../lib/glm/vec3.hxx"
#include "../math/direction.hxx"
#include "../util/types.hxx"

namespace rt::hegem
{
    inline namespace type
    {
        using position_type = glm::vec3;
        using offset_type = glm::vec3;
        using direction_type = math::direction_type;
        using util::types::usize;
    }
}

