#pragma once
#include "hemesh.fwd.hh"
#include "../scene/shape/mesh.hh"

namespace rt::hemesh
{
    scene::shapes::mesh build_mesh(hemesh const& m);
}

