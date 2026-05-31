#pragma once
// Build a local coordinate system (space) out of a single axis.
#include "../lib/glm/fwd.hh"
#include "direction.hh"

namespace rt::math
{
    glm::mat3 local_space(direction_type z);
}

