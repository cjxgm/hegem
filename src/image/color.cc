#include "../lib/glm/op/common.hh"
#include "../lib/glm/op/exp.hh"
#include "../lib/std/string-view.hh"
#include "color.hh"
#include <string>
#include <stdexcept>
#include <cassert>

namespace rt::image::color
{
    namespace
    {
        unsigned char from_hex_digit(char hex)
        {
            if ('0' <= hex && hex <= '9') return hex - '0';
            if ('a' <= hex && hex <= 'f') return hex - 'a' + 10;
            if ('A' <= hex && hex <= 'F') return hex - 'a' + 10;
            throw std::runtime_error{std::string{"Invalid hex digit: "} + hex};
        }

        unsigned char to_srgb_component(lib::string_view hex_component)
        {
            assert(hex_component.size() == 2);
            return (from_hex_digit(hex_component[0]) << 4) | from_hex_digit(hex_component[1]);
        }

        //  0 -> black
        //  1 -> white
        // 10 -> whitest white
        linear_rgb aces_film_tonemap(linear_rgb x)
        {
            constexpr auto a = 2.51f;
            constexpr auto b = 0.03f;
            constexpr auto c = 2.43f;
            constexpr auto d = 0.59f;
            constexpr auto e = 0.14f;
            x = max(x, {0});
            return clamp((x * (a * x + b)) / (x * (c * x + d) + e), {0}, {1});
        }
    }

    srgb to_srgb(linear_rgb const& src)
    {
        // Seems like stb-image-write automatically does gamma correction???
        return clamp(src, {0}, {1}) * linear_rgb{255.0};
    }

    srgb to_srgb(util::as_czstring hex_rrggbb)
    {
        try {
            if (hex_rrggbb.size() != 6) {
                throw std::runtime_error{
                    "Invalid length: " + std::to_string(hex_rrggbb.size())
                };
            }
            auto r = to_srgb_component(hex_rrggbb.view().substr(0, 2));
            auto g = to_srgb_component(hex_rrggbb.view().substr(2, 2));
            auto b = to_srgb_component(hex_rrggbb.view().substr(4, 2));
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
        return gamma_to_internal(linear_rgb{src} / linear_rgb{255.0});
    }

    linear_rgb tonemap(linear_rgb const& src, linear_rgb const& black, linear_rgb const& white)
    {
        return aces_film_tonemap((src - black) / (white - black));
    }

    linear_rgb gamma_to_internal(linear_rgb const& src)
    {
        return pow(src, linear_rgb{2.2});
    }

    linear_rgb gamma_to_display(linear_rgb const& src)
    {
        return pow(src, linear_rgb{1.0 / 2.2});
    }
}

