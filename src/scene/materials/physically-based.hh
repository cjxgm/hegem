#pragma once
#include "../../image/color.hh"

namespace rt::scene::materials
{
    struct physically_based
    {
        using color_type = image::color::linear_rgb;

        color_type albedo;
        color_type reflection;
        float roughness;
        float ior;
    };
}

