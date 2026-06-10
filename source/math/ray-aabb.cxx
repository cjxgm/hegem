#include "ray-aabb.hxx"
#include "../lib/glm/op/common.hxx"
#include "constants.hxx"

namespace rt::math::ray_aabb_details
{
    namespace
    {
        using math::inf;
    }

    float ray_intersect_aabb_from_far(ray_type const& ray, glm::vec3 bmin, glm::vec3 bmax)
    {
        // FIXME: potential NaN problem: 0.0f / 0.0f
        auto extents_from_min = (bmin - ray.origin) / *ray.dir;
        auto extents_from_max = (bmax - ray.origin) / *ray.dir;
        auto min_extents = glm::min(extents_from_min, extents_from_max);
        auto max_extents = glm::max(extents_from_min, extents_from_max);
        auto min_extent = glm::compMax(min_extents);
        auto max_extent = glm::compMin(max_extents);
        if (max_extent >= min_extent && max_extent >= 0)
            return min_extent;
        return inf;
    }
}

