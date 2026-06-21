#pragma once
#include "../tool/as-czstring.hxx"
#include "shape.hxx"

namespace hegem::scene
{
    namespace mesh_loader
    {
        shapes::mesh from_path(tool::as_czstring path);
    }
}

