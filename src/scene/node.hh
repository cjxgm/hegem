#pragma once
#include "../lib/mapbox/variant.hh"
#include "shape.hh"
#include "material.hh"
#include <deque>

namespace rt::scene
{
    namespace nodes
    {
        using mapbox::util::recursive_wrapper;

        struct object
        {
            material_id_type material_id;
            shape_type shape;
        };

        using node_type = mapbox::util::variant<
            object,
            recursive_wrapper<struct group>
        >;

        struct group
        {
            using node_container_type = std::deque<node_type>;

            node_container_type nodes;
        };
    }

    using nodes::node_type;
}

