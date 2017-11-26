#include "graph.hh"

namespace rt::sk
{
    node& graph::emplace(int x, int y, int width, op_id id)
    {
        return nodes.emplace_back(node{
            .x = x,
            .y = y,
            .width = width,
            .instance = {id},
            .metadata = &reg.ops[id],
        });
    }
}

