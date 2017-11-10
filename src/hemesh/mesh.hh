#pragma once
#include "hemesh.fwd.hh"
#include "../scene/shape/mesh.hh"

namespace rt::hemesh
{
    scene::shapes::mesh build_mesh(hemesh const& m);
    scene::shapes::mesh build_outline_mesh(hemesh const& m, float bias=1e-4f);
}

