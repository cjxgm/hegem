#pragma once
#include "../../image/color.hh"

namespace rt::scene::materials
{
    struct solid_color
    {
        using color_type = image::color::linear_rgb;

        solid_color(color_type color)
            : color{color}
        {}

        const color_type color;
    };
}

