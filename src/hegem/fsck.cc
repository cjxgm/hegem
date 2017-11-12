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

            #define SKIP_FREE() \
                if (frees.find(&node) != end(frees)) continue

            #define LINK_CONSISTENT_IF_EQ(STRUCT, X0, X1, DISP0, DISP1) do { \
                for (auto& node: m.STRUCT##s.nodes) { \
                    SKIP_FREE(); \
                    auto x0 = X0; \
                    auto x1 = X1; \
                    if (x0 != x1) { \
                        bad = true; \
                        std::cerr \
                            << "\e[1;31mInconsistent link\e[0m " \
                            << DISP0 << " " << x0 \
                            << " != " \
                            << DISP1 << " " << x1 \
                            << "\n"; \
                    } \
                } \
            } while (false)

            #define CHECK_PARENT_CHILD_CONSISTENCY_FULL(STRUCT, CHILD, PARENT) \
                LINK_CONSISTENT_IF_EQ( \
                    STRUCT, \
                    node.CHILD->PARENT, \
                    &node, \
                    #STRUCT "->" #CHILD "->" #PARENT,  \
                    #STRUCT \
                )

            #define CHECK_PARENT_CHILD_CONSISTENCY(STRUCT, FIELD) \
                CHECK_PARENT_CHILD_CONSISTENCY_FULL(STRUCT, FIELD, STRUCT)

            #define CHECK_PREV_NEXT_CONSISTENCY(STRUCT) \
                LINK_CONSISTENT_IF_EQ( \
                    STRUCT, \
                    node.next->prev, \
                    &node, \
                    #STRUCT "->next->prev",  \
                    #STRUCT \
                )

            CHECK_PARENT_CHILD_CONSISTENCY(body, any_face);
            CHECK_PARENT_CHILD_CONSISTENCY(face, boundary);
            CHECK_PARENT_CHILD_CONSISTENCY(ring, any_hege);
            CHECK_PARENT_CHILD_CONSISTENCY(edge, any_hege);
            CHECK_PARENT_CHILD_CONSISTENCY_FULL(vert, any_hege, start);

            CHECK_PREV_NEXT_CONSISTENCY(body);
            CHECK_PREV_NEXT_CONSISTENCY(face);
            CHECK_PREV_NEXT_CONSISTENCY(ring);
            CHECK_PREV_NEXT_CONSISTENCY(hege);

            #undef SKIP_FREE
            #undef LINK_CONSISTENT_IF_EQ
            #undef CHECK_PARENT_CHILD_CONSISTENCY_FULL
            #undef CHECK_PARENT_CHILD_CONSISTENCY
            #undef CHECK_PREV_NEXT_CONSISTENCY

            std::cerr << "\e[33m----------------------------------------\e[0m\n";
            return bad;
        }
    }
}

