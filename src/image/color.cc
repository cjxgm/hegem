#include "color.hh"
#include <string>
#include <stdexcept>
#include <cassert>

#define WANT_COMMON_OP
#define WANT_EXP_OP
#include "../lib/glm.hh"
#undef WANT_EXP_OP
#undef WANT_COMMON_OP

namespace rt::image::color
{
    srgb to_srgb(linear_rgb const& src)
    {
        return pow(clamp(src, {0}, {1}), linear_rgb{1.0/2.2}) * linear_rgb{255.0};
    }

    namespace
    {
        unsigned char from_hex_digit(char hex)
        {
            if ('0' <= hex && hex <= '9') return hex - '0';
            if ('a' <= hex && hex <= 'f') return hex - 'a' + 10;
            if ('A' <= hex && hex <= 'F') return hex - 'a' + 10;
            throw std::runtime_error{std::string{"Invalid hex digit: "} + hex};
        }

        unsigned char to_srgb_component(gsl::cstring_span<> const& hex_component)
        {
            assert(hex_component.size() == 2);
            return (from_hex_digit(hex_component[0]) << 4) | from_hex_digit(hex_component[1]);
        }
    }

    srgb to_srgb(gsl::cstring_span<> hex_rrggbb)
    {
        try {
            if (hex_rrggbb.size() != 6) {
                throw std::runtime_error{
                    "Invalid length: " + std::to_string(hex_rrggbb.size())
                };
            }
            auto r = to_srgb_component(hex_rrggbb.subspan(0, 2));
            auto g = to_srgb_component(hex_rrggbb.subspan(2, 2));
            auto b = to_srgb_component(hex_rrggbb.subspan(4, 2));
            return {r, g, b};
        }
        catch (std::runtime_error const& ex) {
            throw std::runtime_error{
                "Invalid hex color: " + to_string(hex_rrggbb) + "\n" + ex.what()
            };
        }
    }

    linear_rgb to_linear_rgb(srgb const& src)
    {
        return pow(linear_rgb{src} / linear_rgb{255.0}, linear_rgb{2.2});
    }
}

