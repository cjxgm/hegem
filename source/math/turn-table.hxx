#pragma once
#include "../lib/glm/fwd.hxx"

namespace hegem::math
{
    // Some rotation model
    struct turn_table
    {
        glm::vec2 angles{};
        float zoffset{};
    };

    glm::mat4 rotation(turn_table const& tt);
}

