#include "../lib/imgui.hh"
#include "op-registry.hh"

namespace rt::sk
{
    namespace op
    {
        namespace
        {
            // TODO: rainbow colors in Lch color space
            color_type color_from_hsv(float h, float s, float v)
            {
                auto imcolor = ImVec4{ImColor::HSV(h, s, v)};
                return {
                    imcolor.x,
                    imcolor.y,
                    imcolor.z,
                };
            }
        }

        op_registry::op_registry()
        {
            #define KIND(ID, ...) auto& kind_metadata_##ID = kinds[kind_id::ID];
            #include "op.inl"

            {
                auto kind_index = 0.0f;
                auto kind_count = float(kinds.size());
                #define KIND(ID, NAME, TOOLTIP) \
                { \
                    auto hue = kind_index++ / kind_count; \
                    kind_metadata_##ID = { \
                        .id = #ID, \
                        .name = NAME, \
                        .tooltip = TOOLTIP, \
                        .color_fg = color_from_hsv(hue, 0.5f, 0.8f), \
                        .color_bg = color_from_hsv(hue, 0.3f, 0.3f), \
                        .color_fg_accent = color_from_hsv(hue, 0.4f, 0.5f), \
                        .color_bg_accent = color_from_hsv(hue, 0.3f, 0.2f), \
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

