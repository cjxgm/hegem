#include "op-registry.hh"

namespace rt::sk
{
    namespace op
    {
        op_registry::op_registry()
        {
            #define KIND(ID, ...) auto& kind_metadata_##ID = kinds[kind_id::ID];
            #include "op.inl"

            {
                auto kind_index = 0.0f;
                auto kind_count = float(kinds.size());
                #define KIND(ID, NAME, TOOLTIP) \
                { \
                    auto hue = kind_index++ / kind_count * 0.95f + 0.05f; \
                    kind_metadata_##ID = { \
                        .id = #ID, \
                        .name = NAME, \
                        .tooltip = TOOLTIP, \
                        .palette = { hue }, \
                    }; \
                }
                #include "op.inl"
            }

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

