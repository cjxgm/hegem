#pragma once
#include "../lib/glm/fwd.hxx"
#include "color.hxx"

namespace hegem::image
{
    namespace colorspace
    {
        using color_type = glm::vec3;

        // h ranging from [0, 1] means [0°, 360°], for convenience sake
        auto lch_to_rgb(color_type lch) -> color::linear_rgb;
    }
}

