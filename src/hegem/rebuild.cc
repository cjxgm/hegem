#include "rebuild.hh"
#include "hemesh.hh"
#include <stdexcept>

namespace rt::hegem
{
    template <class T>
    auto make_relocatable_pointer(T*& ptr)
    {
        return std::ref(reinterpret_cast<void*&>(ptr));
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
}

