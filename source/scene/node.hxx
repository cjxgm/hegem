#pragma once
#include "../lib/glm/mat4.hxx"
#include "../lib/mapbox/variant.hxx"
#include "shape.hxx"
#include "material.hxx"
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
            recursive_wrapper<struct xform>,
            recursive_wrapper<struct group>
        >;

        struct xform
        {
            glm::mat4 model;
            node_type node;
        };

        struct group
        {
            using node_container_type = std::deque<node_type>;

            node_container_type nodes;
        };
    }

    using nodes::node_type;
}

