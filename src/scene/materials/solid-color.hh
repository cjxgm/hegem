#pragma once
#include "../../image/color.hh"

namespace rt::scene::materials
{
    struct solid_color
    {
        using color_type = image::color::linear_rgb;

        color_type color;
    };
}

