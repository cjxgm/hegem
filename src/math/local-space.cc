#include "local-space.hh"
#include "../lib/glm/mat3.hh"
#include "../lib/glm/vec3.hh"
#include "../lib/glm/op/geom.hh"

namespace rt::math
{
    glm::mat3 local_space(direction_type z)
    {
        auto fake_x = (z->x * z->x < 0.5 ? glm::vec3{1.0f, 0.0f, 0.0f} : glm::vec3{0.0f, 1.0f, 0.0f});
        auto y = direction_type{cross(*z, fake_x)};
        auto x = direction_type{cross(*y, *z)};
        return { *x, *y, *z };
    }
}

