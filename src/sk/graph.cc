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

    node* graph::find_node(int x, int y, node_id_type ignored)
    {
        for (auto& node: node_range())
            if (node.id != ignored && contains(node, x, y))
                return &node;
        return nullptr;
    }

    std::vector<node*> graph::find_nodes(int x, int y, node_id_type ignored)
    {
        std::vector<node*> found;
        for (auto& node: node_range())
            if (node.id != ignored && contains(node, x, y))
                found.emplace_back(&node);
        return found;
    }

    int graph::find_empty_width(int x, int y, int width, node_id_type ignored)
    {
        for (auto& node: node_range()) {
            if (node.id == ignored) continue;
            if (node.y != y) continue;
            if (node.x + node.width <= x) continue;
            width = std::min(std::max(node.x - x, 0), width);
        }
        return width;
    }

    int graph::find_empty_x(int x, int y, node_id_type ignored)
    {
        while (auto node = find_node(x, y, ignored))
            x = node->x + node->width;
        return x;
    }

    void graph::collect_garbage()
    {
        nodes.erase(
            std::remove_if(
                begin(nodes),
                end(nodes),
                [] (auto& node) { return node.is_garbage; }),
            end(nodes));
    }
}

