#pragma once
#include "../lib/glm/vec3.hxx"
#include "../lib/mapbox/variant.hxx"
#include "../math/direction.hxx"
#include "../image/color.hxx"

namespace hegem::scene
{
    namespace lamps
    {
        using math::direction_type;
        using position_type = glm::vec3;
        using color_type = image::color::linear_rgb;

        struct sun
        {
            direction_type dir;
            color_type color;
        };

        struct omni
        {
            position_type center;
            color_type color;
        };

        using lamp_type = mapbox::util::variant<sun, omni>;
    }

    using lamps::lamp_type;
}

