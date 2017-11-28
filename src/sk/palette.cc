#include "../lib/imgui.hh"
#include "palette.hh"

namespace rt::sk
{
    namespace palette_details
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

        palette_type::palette_type(float hue)
            : fg{color_from_hsv(hue, 0.5f, 0.8f)}
            , bg{color_from_hsv(hue, 0.3f, 0.3f)}
            , fg_accent{color_from_hsv(hue, 0.4f, 0.5f)}
            , bg_accent{color_from_hsv(hue, 0.3f, 0.2f)}
        {}
    }
}

