#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/mapbox/variant.hh"
#include "../math/unit.hh"
#include "../image/color.hh"

namespace rt::scene
{
    namespace lamps
    {
        using direction_type = math::unit<glm::vec3>;
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

