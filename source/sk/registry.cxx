#include "registry.hxx"
#include "../util/unreachable.macro.hxx"
#include <algorithm>

namespace rt::sk
{
    namespace op
    {
        op_registry::op_registry()
        {
            #define KIND(ID, ...) auto& kind_metadata_##ID = kinds[kind_id::ID];
            #define SECTION(ID, ...) auto& kind_metadata_section_##ID = kinds[kind_id::section_##ID];
            #include "op.inl"

            {
                auto kind_index = 0.0f;
                auto max_kind = std::max(1.0f, float(kinds.size()) - 1.0f);
                #define KIND(ID, NAME, TOOLTIP) \
                { \
                    auto hue = kind_index++ / max_kind * 0.70f + 0.15f; \
                    kind_metadata_##ID = { \
                        .id = #ID, \
                        .name = NAME, \
                        .tooltip = TOOLTIP, \
                        .palette = { hue }, \
                    }; \
                }
                #define SECTION(ID, NAME, TOOLTIP) \
                { \
                    auto hue = kind_index++ / max_kind * 0.70f + 0.15f; \
                    kind_metadata_section_##ID = { \
                        .id = #ID, \
                        .name = "---- " NAME " ----", \
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
            #define SECTION(ID, ...) \
                ops[op_id::section_##ID##_##ID] = { \
                    .kind = &kind_metadata_section_##ID, \
                    .id = STRINGIFY(ID##_##ID), \
                    .name = "", \
                    .tooltip = "", \
                    .arity = 0, \
                };
            #include "op.inl"
        }
    }
}

