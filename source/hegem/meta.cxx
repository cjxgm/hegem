#include "../lib/boost/format.hxx"
#include "meta.hxx"
#include "hemesh.hxx"
#include "list.hxx"
#include <stdexcept>

namespace rt::hegem
{
    inline namespace meta
    {
        namespace
        {
            template <class T>
            auto make_relocatable_pointer(T*& ptr)
            {
                return std::ref(reinterpret_cast<void*&>(ptr));
            }
        }

        pointer_list_type collect_pointers(hemesh & m)
        {
            pointer_list_type ptrs{
                make_relocatable_pointer(m.any_body),
            };

            #define STRUCT(TYPE, VAR) \
                for (auto& free: m.VAR##s.frees) \
                    ptrs.emplace_back(make_relocatable_pointer(free)); \
                for (auto& node: m.VAR##s.nodes) {
            #define END_STRUCT() \
                }
            #define FIELD_PTR_FROM_SLAB(TYPE, VAR) \
                ptrs.emplace_back(make_relocatable_pointer(node.VAR));
            #include "primitive.inl"

            return ptrs;
        }

        pointer_name_map_type build_pointer_name_map(hemesh const& m)
        {
            std::unordered_map<std::string, int> counters;
            std::unordered_map<void const*, std::string> pointer_names;

            auto fmt_name_3 = boost::format("%s-%03x");
            auto fmt_name_4 = boost::format("%s-%04x");
            auto fmt_name_8 = boost::format("%s-%08x");
            // fmt_name_16? No I don't think it's necessary.
            // If there are actually that large number, just let it over-align.

            #define STRUCT(TYPE, VAR) \
            { \
                std::string var{#VAR}; \
                for (auto& node: m.VAR##s.nodes) { \
                    if (counters.find(var) == end(counters)) \
                        counters.emplace(var, 0); \
                    auto id = counters[var]++; \
                    auto& fmt = ( \
                        id > 0xFFFF ? fmt_name_8 : \
                        id > 0xFFF   ? fmt_name_4 : \
                        fmt_name_3); \
                    pointer_names.emplace(&node, str(fmt % #VAR % id)); \
                } \
            }
            #include "primitive.inl"

            return pointer_names;
        }

        pointer_set_type build_free_pointer_set(hemesh const& m)
        {
            pointer_set_type frees;

            #define STRUCT(TYPE, VAR) \
                for (auto free: m.VAR##s.frees) \
                    frees.emplace(free);
            #include "primitive.inl"

            return frees;
        }

        pointer_map_type build_pointer_map(hemesh const& old, hemesh & now)
        {
            pointer_map_type pmap;

            #define STRUCT(TYPE, VAR) \
            { \
                auto& old_slab = old.VAR##s; \
                auto& now_slab = now.VAR##s; \
                if (old_slab.capacity() != now_slab.capacity()) { \
                    throw std::logic_error{ \
                        "Cannot build pointer map out of unmatching hemeshes. " \
                        "Their " #VAR " slabs differ in capacity." \
                    }; \
                } \
                if (old_slab.size() != now_slab.size()) { \
                    throw std::logic_error{ \
                        "Cannot build pointer map out of unmatching hemeshes. " \
                        "Their " #VAR " slabs differ in size." \
                    }; \
                } \
                auto old_it = begin(old_slab.nodes); \
                auto now_it = begin(now_slab.nodes); \
                auto cap = old_slab.capacity(); \
                for (auto i=0; i<cap; i++) { \
                    auto old_ptr = static_cast<void const*>(&*old_it++); \
                    auto now_ptr = static_cast<void      *>(&*now_it++); \
                    pmap[old_ptr] = now_ptr; \
                } \
            }
            #include "primitive.inl"

            return pmap;
        }

        void reconstruct_pointers(pointer_map_type const& pmap, pointer_list_type const& ptrs)
        {
            for (auto p: ptrs) {
                if (p == nullptr) continue;

                auto entry_it = pmap.find(p);
                if (entry_it == end(pmap))
                    throw std::logic_error{"Invalid pointer map. There are missing entries."};

                p.get() = entry_it->second;
            }
        }

        void extend(hemesh const& old, hemesh & now)
        {
            if (old.any_body == nullptr)
                return;

            auto frees = build_free_pointer_set(old);
            pointer_map_type pmap;
            pointer_list_type ptrs;

            #define STRUCT(TYPE, VAR) \
            { \
                auto& old_slab = old.VAR##s; \
                auto& now_slab = now.VAR##s; \
                for (auto& node: old_slab.nodes) { \
                    if (frees.find(&node) != end(frees)) continue; \
                    auto& now_node = *now_slab.alloc(); \
                    now_node = node; \
                    pmap[&node] = &now_node;
            #define END_STRUCT() \
                } \
            }
            #define FIELD_PTR_FROM_SLAB(TYPE, VAR) \
                ptrs.emplace_back(make_relocatable_pointer(now_node.VAR));
            #include "primitive.inl"

            reconstruct_pointers(pmap, ptrs);

            auto extra_body = static_cast<body_type*>(pmap.at(old.any_body));
            list::extend(now.any_body, extra_body);
        }
    }
}

