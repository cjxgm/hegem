#include "serialize.hxx"
#include "hemesh.hxx"
#include "meta.hxx"

namespace hegem::swing
{
    void serialize(hemesh const& m, serializer& sr)
    {
        auto frees = build_free_pointer_set(m);

        // Declare structures (skipping nodes in freelist)
        sr.declare_structure("hemesh", "mesh", &m);
        #define STRUCT(TYPE, VAR) \
            for (auto& node: m.VAR##s.nodes) { \
                if (frees.find(&node) != end(frees)) continue; \
                sr.declare_structure(#TYPE, #VAR, &node); \
            }
        #include "primitive.inl"

        // Serialize root structure
        sr.begin_structure("hemesh", "mesh", &m);
        sr.field_ptr_from_slab("body_type", "any_body", m.any_body);
        sr.end_structure();

        // Serialize (skipping nodes in freelist)
        #define STRUCT(TYPE, VAR) \
            { \
                for (auto& node: m.VAR##s.nodes) { \
                    if (frees.find(&node) != end(frees)) continue; \
                    sr.begin_structure(#TYPE, #VAR, &node);
        #define END_STRUCT() \
                    sr.end_structure(); \
                } \
            }
        #define FIELD_PTR_FROM_SLAB(TYPE, VAR) \
            sr.field_ptr_from_slab(#TYPE, #VAR, node.VAR);
        #define FIELD(TYPE, VAR) \
            sr.field(#TYPE, #VAR, node.VAR);
        #include "primitive.inl"
    }
}

