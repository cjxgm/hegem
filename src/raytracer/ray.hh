#pragma once
#include "../lib/glm/vec3.hh"
#include "../math/unit.hh"

namespace rt::raytracer
{
    struct ray_type
    {
        using point_type = glm::vec3;
        using direction_type = math::unit<glm::vec3>;

        const point_type origin;
        const direction_type dir;

        point_type at(float extent) const { return origin + (*dir) * extent; }
    };
}

