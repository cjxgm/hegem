#pragma once
#include "../util/constraints.hxx"
#include "../util/range.hxx"
#include "op.hxx"
#include "node.hxx"
#include "registry.hxx"
#include <vector>

namespace hegem::skein
{
    struct graph: util::constraints::only_movable
    {
        auto emplace(int x, int y, int width, op_id id) -> node&;
        auto find_node(node_id_type id) -> node*;
        auto find_node(int x, int y) -> node*;                      // Skips ghost nodes.
        auto find_empty_width(int x, int y, int max_width) -> int;  // Skips ghost nodes.
        auto find_empty_x(int x, int y) -> int;                     // Skips ghost nodes.

        auto inputs_of(node const* n) -> std::vector<node*>;
        auto count_inputs_of(node const* n) -> int;

        // n will NOT be contained in the result.
        auto stack_of(node const* n, bool strict) -> std::vector<node*>;

        auto node_range() { return util::range{begin(nodes), end(nodes)}; }
        auto op_metadata_range() { return util::range{begin(reg.ops), end(reg.ops)}; }

        void collect_garbage();

    private:
        op_registry reg;
        std::vector<node> nodes;
        node_id_type latest_id;
    };
}

