#include "serialize.hh"
#include "hemesh.hh"
#include <unordered_set>

namespace rt::hemesh
{
    void serialize(hemesh const& m, serializer& sr)
    {
        std::unordered_set<void const*> frees;

        // Build free set (ID == 0 means it's a free pointer)
        #define STRUCT(TYPE, VAR) \
            for (auto free: m.VAR##s.frees) \
                frees.emplace(free);
        #include "primitive.inl"

        // Serialize root structure
        sr.begin_structure("mesh", &m);
        sr.field_ptr_from_slab("any_body", m.any_body);
        sr.end_structure();

        // Serialize (skipping nodes in freelist)
        #define STRUCT(TYPE, VAR) \
            { \
                for (auto& node: m.VAR##s.nodes) { \
                    if (frees.find(&node) != end(frees)) continue; \
                    sr.begin_structure(#VAR, &node);
        #define END_STRUCT() \
                    sr.end_structure(); \
                } \
            }
        #define FIELD_PTR_FROM_SLAB(TYPE, VAR) \
            sr.field_ptr_from_slab(#VAR, node.VAR);
        #define FIELD(TYPE, VAR) \
            sr.field(#VAR, node.VAR);
        #include "primitive.inl"
    }
}

