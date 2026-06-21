#include "../image/colorspace.hxx"
#include "palette.hxx"

namespace hegem::skein
{
    namespace palette_details
    {
        namespace
        {
            color_type color_from_hcl(float h, float c, float l)
            {
                return image::colorspace::lch_to_rgb({ l, c, h });
            }
        }

        palette_type::palette_type(float hue)
            : fg{color_from_hcl(hue, 20.0f, 65.0f)}
            , bg{color_from_hcl(hue, 2.0f, 25.0f)}
            , fg_accent{color_from_hcl(hue, 3.0f, 40.0f)}
            , bg_accent{color_from_hcl(hue, 2.0f, 10.0f)}
        {}
    }
}

