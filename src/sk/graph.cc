#include "graph.hh"
#include <algorithm>

namespace rt::sk
{
    node& graph::emplace(int x, int y, int width, op_id id)
    {
        return nodes.emplace_back(node{
            .id = ++latest_id,
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

    int graph::find_empty_width(int x, int y, int width)
    {
        for (auto& node: node_range()) {
            if (node.y != y) continue;
            if (node.x + node.width <= x) continue;
            width = std::min(std::max(node.x - x, 0), width);
        }
        return width;
    }
}

