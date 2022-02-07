#include "graph.hh"
#include <algorithm>
#include <unordered_set>

namespace rt::sk
{
    auto graph::emplace(int x, int y, int width, op_id id) -> node&
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

    auto graph::find_node(node_id_type id) -> node*
    {
        for (auto& node: node_range())
            if (node.id == id)
                return &node;
        return nullptr;
    }

    auto graph::find_node(int x, int y, node_id_type ignored) -> node*
    {
        for (auto& node: node_range())
            if (node.id != ignored && contains(node, x, y))
                return &node;
        return nullptr;
    }

    auto graph::find_empty_width(int x, int y, int width, node_id_type ignored) -> int
    {
        for (auto& node: node_range()) {
            if (node.id == ignored) continue;
            if (node.y != y) continue;
            if (node.x + node.width <= x) continue;
            width = std::min(std::max(node.x - x, 0), width);
        }
        return width;
    }

    auto graph::find_empty_x(int x, int y, node_id_type ignored) -> int
    {
        while (auto node = find_node(x, y, ignored))
            x = node->x + node->width;
        return x;
    }

    auto graph::inputs_of(node const* n) -> std::vector<node*>
    {
        std::vector<node*> result;

        for (auto& input: node_range())
            if (connects(input, *n))
                result.emplace_back(&input);

        std::sort(
            begin(result),
            end(result),
            [] (auto a, auto b) { return a->x < b->x; });

        return result;
    }

    auto graph::count_inputs_of(node const* n) -> int
    {
        int result = 0;

        for (auto& input: node_range())
            if (connects(input, *n))
                result++;

        return result;
    }

    auto graph::stack_of(node const* n, bool strict) -> std::vector<node*>
    {
        std::unordered_set<node*> stacked;
        std::unordered_set<node*> pending{const_cast<node*>(n)};

        while (!pending.empty()) {
            auto nodes = std::vector<node*>{pending.begin(), pending.end()};
            stacked.insert(pending.begin(), pending.end());
            pending.clear();

            for (auto& candidate: node_range()) {
                if (stacked.count(&candidate) != 0u)
                    continue;

                for (auto known: nodes) {
                    if (connects(candidate, *known))
                        pending.insert(&candidate);

                    if (!strict && candidate.y <= n->y && connects(*known, candidate))
                        pending.insert(&candidate);
                }
            }
        }

        stacked.erase(const_cast<node*>(n));
        auto result = std::vector<node*>{stacked.begin(), stacked.end()};
        stacked.clear();

        std::sort(
            begin(result),
            end(result),
            [] (auto a, auto b) {
                if (a->y > b->y) return true;
                if (a->y < b->y) return false;
                return (a->x < b->x);
            }
        );

        return result;
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

