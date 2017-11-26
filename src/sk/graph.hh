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

        auto node_range() { return util::range{begin(nodes), end(nodes)}; }
        auto op_metadata_range() { return util::range{begin(reg.ops), end(reg.ops)}; }

    private:
        op_registry reg;
        std::vector<node> nodes;
    };
}

