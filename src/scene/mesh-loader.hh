#pragma once
#include "../util/as-czstring.hh"
#include "shape.hh"

namespace rt::scene
{
    namespace mesh_loader
    {
        shapes::mesh from_path(util::as_czstring path);
    }
}

