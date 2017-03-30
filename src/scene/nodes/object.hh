#pragma once
#include "../geometry.hh"
#include "../material.hh"

namespace rt::scene::nodes
{
    struct object
    {
        material_id_type material_id;
        geometry_type geometry;
    };
}

