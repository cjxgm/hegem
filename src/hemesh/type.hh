#pragma once
// basic types
#include "../lib/glm/vec3.hh"
#include "../math/direction.hh"
#include "../util/types.hh"

namespace rt::hemesh
{
    inline namespace type
    {
        using position_type = glm::vec3;
        using offset_type = glm::vec3;
        using direction_type = math::direction_type;
        using util::types::usize;
    }
}

