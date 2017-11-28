#pragma once
#include <cstddef>
#include "op.hh"
#include "op-registry.hh"

namespace rt::sk
{
    using node_id_type = std::size_t;

    struct node
    {
        node_id_type id;
        int x;
        int y;
        int width;
        op_instance instance;

        // cache
        op_metadata const* metadata;

        // flags
        bool is_garbage{};
    };

    bool contains(node const& n, int x, int y);
    bool overlaps(node const& a, node const& b);
    bool connects(node const& above, node const& below);
}

