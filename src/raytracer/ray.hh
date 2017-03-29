#pragma once
#include "../math/unit.hh"

#define WANT_VEC3
#include "../lib/glm.hh"
#undef WANT_VEC3

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

