#pragma once
#include "../lib/mapbox/variant.hh"
#include "node.hh"
#include "view.hh"
#include "material.hh"
#include <deque>

namespace rt::scene
{
    struct scene_type
    {
        using material_container_type = std::deque<material_type>;
        using view_container_type = std::deque<view_type>;

        material_container_type materials;
        view_container_type views;
        material_id_type environment;
        node_type root;
    };
}

