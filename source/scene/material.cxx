#include "../lib/glm/op/geom.hxx"
#include "material.hxx"

namespace rt::scene::materials
{
    bool reflective(material_type const& mat)
    {
        return mat.match(
                [] (solid_color const&) { return false; },
                [] (phong const& mat) { return dot(mat.reflection, mat.reflection) != 0; },
                [] (physically_based const&) { return true; });
    }

    bool refractive(material_type const& mat)
    {
        return mat.match(
                [] (solid_color const&) { return false; },
                [] (phong const& mat) { return (mat.ior >= 1.0f); },
                [] (physically_based const& mat) { return (mat.opacity < 1.0f); });
    }

    float index_of_refraction(material_type const& mat)
    {
        return mat.match(
                [] (solid_color const&) { return 0.0f; },
                [] (phong const& mat) { return mat.ior; },
                [] (physically_based const& mat) { return mat.ior; });
    }

    float roughness(material_type const& mat)
    {
        return mat.match(
                [] (solid_color const&) { return 0.0f; },
                [] (phong const& mat) { return mat.roughness; },
                [] (physically_based const& mat) { return mat.roughness; });
    }
}

