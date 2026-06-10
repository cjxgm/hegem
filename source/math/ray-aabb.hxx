#pragma once
#include "../lib/glm/vec3.hxx"
#include "../raytracer/ray.hxx"

namespace rt::math
{
    namespace ray_aabb_details
    {
        using rt::raytracer::ray_type;

        // Pretend the ray's origin is at `ray.at(-inf)`,
        // then intersect the ray with the aabb defined in [bmin, bmax].
        //
        // Returns +inf if not intersected
        // Returns the extent so that ray.at(extent) is the hit point otherwise.
        float ray_intersect_aabb_from_far(ray_type const& ray, glm::vec3 bmin, glm::vec3 bmax);
    }

    using ray_aabb_details::ray_intersect_aabb_from_far;
}

