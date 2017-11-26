#pragma once
#include "../util/constraints.hh"
#include "../util/range.hh"
#include "op.hh"
#include "node.hh"
#include "op-registry.hh"
#include <vector>

namespace rt::sk
{
    struct graph: util::constraints::only_movable
    {
        node& emplace(int x, int y, int width, op_id id);
        node* find_node(node_id_type id);
        node* find_node(int x, int y);
        std::vector<node*> find_nodes(int x, int y);
        int find_empty_width(int x, int y, int max_width);

        auto node_range() { return util::range{begin(nodes), end(nodes)}; }
        auto op_metadata_range() { return util::range{begin(reg.ops), end(reg.ops)}; }

    private:
        op_registry reg;
        std::vector<node> nodes;
        node_id_type latest_id;
    };
}

