#include "../lib/glm/vec3.hxx"
#include "../math/constants.hxx"
#include "colorspace.hxx"
#include <cmath>

namespace hegem::image::colorspace
{
    // Adapted from https://gist.github.com/stepheneb/3019585
    namespace
    {
        // h ranging from [0, 1] means [0°, 360°], for convenience sake
        auto lch_lab(color_type lch) -> color_type
        {
            auto h = lch[2] * math::pi * 2.0f;
            return {
                lch[0],
                std::cos(h) * lch[1],
                std::sin(h) * lch[1],
            };
        }

        auto lab_xyz(float x) -> float
        {
            return x > 0.206893034f
                ? x * x * x
                : (x - 4.0f / 29.0f) / 7.787037f;
        }

        auto xyz_rgb(float x) -> float
        {
            return x <= 0.00304f
                ? 12.92f * x
                : 1.055f * std::pow(x, 1.0f / 2.4f) - 0.055f;
        }

        auto lab_rgb(color_type lab) -> color::linear_rgb
        {
            // D65 standard referent
            static constexpr auto xref = 0.950470f;
            static constexpr auto yref = 1.000000f;
            static constexpr auto zref = 1.088830f;

            auto y = (lab[0] + 16.0f) / 116.0f;
            auto x = y + lab[1] / 500.0f;
            auto z = y - lab[2] / 200.0f;

            x = lab_xyz(x) * xref;
            y = lab_xyz(y) * yref;
            z = lab_xyz(z) * zref;

            return color::linear_rgb{
                xyz_rgb( 3.2404542 * x + -1.5371385 * y + -0.4985314 * z),
                xyz_rgb(-0.9692660 * x +  1.8760108 * y +  0.0415560 * z),
                xyz_rgb( 0.0556434 * x + -0.2040259 * y +  1.0572252 * z),
            };
        }
    }

    auto lch_to_rgb(color_type lch) -> color::linear_rgb
    {
        return lab_rgb(lch_lab(lch));
    }
}

