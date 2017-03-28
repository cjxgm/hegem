#pragma once
#include "../../image/color.hh"

namespace rt::scene::materials
{
    // TODO
    struct physically_based
    {
        using color_type = image::color::linear_rgb;

        const color_type albedo_color;
        const color_type reflection_color;
        const float ior;
        const float glossiness;
    };
}

