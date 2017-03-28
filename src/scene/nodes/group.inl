#pragma once
#include <deque>

namespace rt::scene::nodes
{
    struct group
    {
        using node_container_type = std::deque<node_type>;

        node_container_type nodes;
    };
}

