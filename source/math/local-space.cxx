#include "local-space.hxx"
#include "../lib/glm/mat3.hxx"
#include "../lib/glm/vec3.hxx"
#include "../lib/glm/op/geom.hxx"

namespace hegem::math
{
    glm::mat3 local_space(direction_type z)
    {
        auto axis = (z->z * z->z < 1e-6f ? glm::vec3{0.0f, 0.0f, 1.0f} : glm::vec3{1.0f, 0.0f, 0.0f});
        auto y = direction_type{cross(*z, axis)};
        auto x = direction_type{cross(*y, *z)};
        return { *x, *y, *z };
    }
}

