#pragma once
#include "../../image/color.hh"

namespace rt::scene::materials
{
    // TODO
    struct physically_based
    {
        using color_type = image::color::linear_rgb;

        color_type albedo_color;
        color_type reflection_color;
        float ior;
        float glossiness;
    };
}

