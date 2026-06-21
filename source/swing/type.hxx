#pragma once
// basic types
#include "../lib/glm/vec3.hxx"
#include "../math/direction.hxx"
#include "../tool/types.hxx"

namespace hegem::swing
{
    inline namespace type
    {
        using position_type = glm::vec3;
        using offset_type = glm::vec3;
        using direction_type = math::direction_type;
        using tool::types::usize;
    }
}

