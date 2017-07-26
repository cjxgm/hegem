#pragma once
#include "../lib/mapbox/variant.hh"
#include "../image/color.hh"

namespace rt::scene
{
    namespace texture_packs
    {
        using color_type = image::color::linear_rgb;

        struct pure
        {
        };

        struct checkerboard
        {
            color_type accent;
            float size;
        };

        using texture_pack_type = mapbox::util::variant<
            checkerboard,
            pure
        >;

        color_type sample_albedo(texture_pack_type const& tpack, color_type const& albedo, glm::vec3 const& pos);
    }

    using texture_packs::texture_pack_type;
}

