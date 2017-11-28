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
        node* find_node(int x, int y, node_id_type ignored=0);
        std::vector<node*> find_nodes(int x, int y, node_id_type ignored=0);
        int find_empty_width(int x, int y, int max_width, node_id_type ignored=0);
        int find_empty_x(int x, int y, node_id_type ignored=0);

        auto inputs_of(node const* n) -> std::vector<node*>;
        auto count_inputs_of(node const* n) -> int;

        auto node_range() { return util::range{begin(nodes), end(nodes)}; }
        auto op_metadata_range() { return util::range{begin(reg.ops), end(reg.ops)}; }

        void collect_garbage();

    private:
        op_registry reg;
        std::vector<node> nodes;
        node_id_type latest_id;
    };
}

