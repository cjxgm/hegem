#pragma once
#include "../util/as-czstring.hxx"
#include "shape.hxx"

namespace hegem::scene
{
    namespace mesh_loader
    {
        shapes::mesh from_path(util::as_czstring path);
    }
}

