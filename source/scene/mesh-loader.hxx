#pragma once
#include "../util/as-czstring.hxx"
#include "shape.hxx"

namespace rt::scene
{
    namespace mesh_loader
    {
        shapes::mesh from_path(util::as_czstring path);
    }
}

