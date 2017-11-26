#include "graph.hh"

namespace rt::sk
{
    node& graph::emplace(int x, int y, int width, op_id id)
    {
        return nodes.emplace_back(node{
            .id = next_id++,
            .x = x,
            .y = y,
            .width = width,
            .instance = {id},
            .metadata = &reg.ops[id],
        });
    }

    node* graph::find_node(node_id_type id)
    {
        for (auto& node: node_range())
            if (node.id == id)
                return &node;
        return nullptr;
    }

    node* graph::find_node(int x, int y)
    {
        for (auto& node: node_range())
            if (contains(node, x, y))
                return &node;
        return nullptr;
    }

    std::vector<node*> graph::find_nodes(int x, int y)
    {
        std::vector<node*> found;
        for (auto& node: node_range())
            if (contains(node, x, y))
                found.emplace_back(&node);
        return found;
    }
}

