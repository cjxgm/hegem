#include "fsck.hh"
#include "hemesh.hh"
#include "meta.hh"
#include "list.hh"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <deque>

namespace rt::hegem
{
    inline namespace fsck
    {
        bool fsck_all(hemesh const& m)
        {
            if (fsck_pointers(m)) return true;
            if (fsck_link_consistency(m)) return true;
            return false;
        }

        bool fsck_pointers(hemesh const& m)
        {
            std::cerr << "\e[1;33m-------- fsck: pointers --------\e[0m\n";
            auto bad = false;

            auto unreachables = build_pointer_name_map(m);
            auto frees = build_free_pointer_set(m);

            // Mark nodes in free lists as reachable
            for (auto free: frees)
                unreachables.erase(free);

            // Build reachability map
            std::unordered_map<void const*, std::unordered_set<void const*>> reachable_map;
            #define STRUCT(TYPE, VAR) \
                for (auto& node: m.VAR##s.nodes) { \
                    if (frees.find(&node) != end(frees)) continue; \
                    auto struct_name = #VAR;
            #define END_STRUCT() \
                }
            #define FIELD_PTR_FROM_SLAB(TYPE, VAR) \
                if (node.VAR == nullptr) { \
                    bad = true; \
                    std::cerr \
                        << "\e[1;31mNull pointer\e[0m " \
                        << struct_name << "." #VAR " = (" << #TYPE << "*) nullptr" \
                        << "\n"; \
                } else { \
                    auto it = unreachables.find(node.VAR); \
                    if (it == end(unreachables)) { \
                        bad = true; \
                        std::cerr \
                            << "\e[1;31mDangling pointer\e[0m " \
                            << struct_name << "." #VAR " = (" << #TYPE << "*) " \
                            << static_cast<void const*>(node.VAR) \
                            << "\n"; \
                    } else { \
                        reachable_map[&node].emplace(node.VAR); \
                    } \
                }
            #include "primitive.inl"

            // Filter out reachable nodes
            {
                std::deque<void const*> pending{m.any_body};
                while (!pending.empty()) {
                    auto u = pending.front();
                    pending.pop_front();

                    for (auto v: reachable_map[u]) {
                        auto it = unreachables.find(v);
                        if (it != end(unreachables)) {
                            unreachables.erase(it);
                            pending.emplace_back(v);
                        }
                    }
                }
            }

            for (auto& entry: unreachables) {
                bad = true;
                std::cerr
                    << "\e[1;31mUnreachable node\e[0m "
                    << entry.first << ": " << entry.second
                    << "\n";
            }

            std::cerr << "\e[33m--------------------------------\e[0m\n";
            return bad;
        }

        bool fsck_link_consistency(hemesh const& m)
        {
            std::cerr << "\e[1;33m-------- fsck: link consistency --------\e[0m\n";
            auto bad = false;
            auto frees = build_free_pointer_set(m);

            #define CHECK_EQ(A, B, NAME) do { \
                auto x0 = A; \
                auto x1 = B; \
                if (x0 != x1) { \
                    bad = true; \
                    std::cerr \
                        << "\e[1;31mInconsistent link\e[0m " \
                        << struct_name << NAME << " " << x0 \
                        << " != " \
                        << struct_name << " " << x1 \
                        << "\n"; \
                } \
            } while (false)

            #define STRUCT(TYPE, VAR) \
                for (auto& node: m.VAR##s.nodes) { \
                    if (frees.find(&node) != end(frees)) continue; \
                    auto struct_name = #VAR; \
                    (void) struct_name;
            #define END_STRUCT() \
                }
            #define CHILD_PARENT_RELATION(CHILD, PARENT) \
                CHECK_EQ(node.CHILD->PARENT, &node, "->" #CHILD "->" #PARENT);
            #include "primitive.inl"

            #define STRUCT(TYPE, VAR) \
                for (auto& node: m.VAR##s.nodes) { \
                    if (frees.find(&node) != end(frees)) continue; \
                    auto struct_name = #VAR; \
                    (void) struct_name;
            #define END_STRUCT() \
                }
            #define CHILDREN_PARENT_RELATION_CUSTOM_NEXT(CHILD_FIRST, PARENT, NEXT) \
            { \
                auto first = node.CHILD_FIRST; \
                auto child = first; \
                do { \
                    CHECK_EQ(child->PARENT, &node, "~>" #CHILD_FIRST "->" #PARENT); \
                    child = child->NEXT; \
                } while (child != first); \
            }
            #include "primitive.inl"

            std::cerr << "\e[33m----------------------------------------\e[0m\n";
            return bad;
        }
    }
}

