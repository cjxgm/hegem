#include "op-registry.hh"

namespace rt::sk
{
    namespace op
    {
        op_registry::op_registry()
        {
            #define KIND(ID, ...) auto& kind_metadata_##ID = kinds[kind_id::ID];
            #include "op.inl"

            #define KIND(ID, NAME, TOOLTIP) \
                kind_metadata_##ID = { \
                    .id = #ID, \
                    .name = NAME, \
                    .tooltip = TOOLTIP, \
                    .color = color_type{0.3f}, \
                };
            #include "op.inl"

            #define STRINGIFY(X) #X
            #define OP(KIND, ID, ARITY, NAME, TOOLTIP, ...) \
                ops[op_id::KIND##_##ID] = { \
                    .kind = &kind_metadata_##KIND, \
                    .id = STRINGIFY(KIND##_##ID), \
                    .name = NAME, \
                    .tooltip = TOOLTIP, \
                    .arity = ARITY, \
                };
            #include "op.inl"
        }
    }
}

