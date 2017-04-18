#pragma once
#include "../lib/glm/fwd.hh"
#include "../util/as-czstring.hh"

namespace rt::image
{
    namespace color
    {
        using linear_rgb = glm::vec3;
        using srgb = glm::tvec3<unsigned char, glm::defaultp>;

        srgb to_srgb(linear_rgb const& src);
        srgb to_srgb(util::as_czstring hex_rrggbb);
        linear_rgb to_linear_rgb(srgb const& src);
        linear_rgb tonemap(linear_rgb const& src, linear_rgb const& black, linear_rgb const& white);
        linear_rgb gamma_to_internal(linear_rgb const& src);
        linear_rgb gamma_to_display(linear_rgb const& src);
    }
}

