#pragma once
#include "../shape.hh"
#include "../material.hh"

namespace rt::scene::nodes
{
    struct object
    {
        material_id_type material_id;
        shape_type shape;
    };
}

