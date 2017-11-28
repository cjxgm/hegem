#pragma once
#include "../lib/glm/vec3.hh"
#include "../image/color.hh"

namespace rt::sk
{
    namespace palette_details
    {
        using color_type = image::color::linear_rgb;

        struct palette_type
        {
            color_type fg;
            color_type bg;
            color_type fg_accent;
            color_type bg_accent;

            palette_type() = default;
            palette_type(float hue);
        };
    }

    using palette_details::palette_type;
    using palette_color_type = palette_details::color_type;
}

