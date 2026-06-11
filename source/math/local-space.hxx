#pragma once
// Build a local coordinate system (space) out of a single axis.
#include "../lib/glm/fwd.hxx"
#include "direction.hxx"

namespace hegem::math
{
    glm::mat3 local_space(direction_type z);
}

