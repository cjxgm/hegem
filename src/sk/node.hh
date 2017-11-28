#pragma once
#include "op.hh"
#include "op-registry.hh"
#include <cstddef>

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
        op_metadata const* metadata;
        bool is_garbage{};
    };

    bool contains(node const& n, int x, int y);
    bool overlaps(node const& a, node const& b);
    bool connects(node const& above, node const& below);
}

