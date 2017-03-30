#pragma once
#include "../../image/color.hh"

namespace rt::scene::materials
{
    struct phong
    {
        using color_type = image::color::linear_rgb;

        color_type diffuse;
    };
}

