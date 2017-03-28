#pragma once
#include "../geometry.hh"
#include "../material.hh"

namespace rt::scene::nodes
{
    struct object
    {
        const material_id_type material_id;
        const geometry_type geometry;
    };
}

