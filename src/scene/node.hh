#pragma once
#include "../lib/mapbox/variant.hh"
#include "nodes/object.hh"

namespace rt::scene
{
    namespace nodes
    {
        using mapbox::util::recursive_wrapper;

        using node_type = mapbox::util::variant<
            object,
            recursive_wrapper<struct group>
        >;
    }

    using nodes::node_type;
}

#include "nodes/group.inl"

