#pragma once
#include "op.hxx"
#include "registry.hxx"
#include <cstddef>
#include <string>

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
        bool is_ghost{};    // invisible to various searching algorithms.
        std::string sanity_error;
        std::string runtime_error;
    };

    bool contains(node const& n, int x, int y);
    bool overlaps(node const& a, node const& b);
    bool connects(node const& above, node const& below);
}

