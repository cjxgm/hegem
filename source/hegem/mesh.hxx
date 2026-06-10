#pragma once
#include "hemesh.fwd.hxx"
#include "../scene/shape/mesh.hxx"

namespace rt::hegem
{
    scene::shapes::mesh build_mesh(hemesh const& m, float bias=0.0f);
    scene::shapes::mesh build_outline_mesh(hemesh const& m, float bias=1e-4f);
}

