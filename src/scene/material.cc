#include "../lib/glm/op/geom.hh"
#include "material.hh"

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
        // TODO
        (void)mat;
        return false;
    }
}

