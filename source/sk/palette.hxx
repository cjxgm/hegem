#pragma once
#include "../lib/glm/vec3.hxx"
#include "../image/color.hxx"

namespace hegem::sk
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

            palette_type(): palette_type{0.0f} {}
            palette_type(float hue);
        };
    }

    using palette_details::palette_type;
    using palette_color_type = palette_details::color_type;
}

