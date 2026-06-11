#pragma once
#include "../lib/glm/vec2.hxx"
#include "../lib/glm/relational.hxx"
#include "../util/as-czstring.hxx"
#include "color.hxx"
#include <vector>
#include <utility>
#include <type_traits>

namespace hegem::image
{
    namespace image_impl
    {
        using namespace color;

        template <class T>
        struct image
        {
            using color_type = T;
            using position_type = glm::ivec2;
            using dimension_type = glm::ivec2;
            using copyable_color_type = T;

            template <class Color, class = std::enable_if_t<std::is_copy_assignable<Color>::value>>
            image(
                    dimension_type size,
                    Color&& fill)
                : size_{size}
                , pixels(size.x * size.y, fill)
            {}

            image(dimension_type size)
                : size_{size}
                , pixels(size.x * size.y)
            {}

            void put_repeat(position_type const& pos, color_type color)
            {
                put_bounded(wrap_around(pos), std::move(color));
            }

            void put_clamp(position_type const& pos, color_type color)
            {
                if (in_bounds(pos)) put_bounded(pos, std::move(color));
            }

            auto& size() const { return size_; }

            friend image<srgb> to_srgb(image<linear_rgb> const& src);
            friend void write(image<srgb> const& src, util::as_czstring output_path);
            friend image<linear_rgb> half(image<linear_rgb> const& src);
            friend image<linear_rgb> tonemap(
                    image<linear_rgb> const& src,
                    linear_rgb const& black,
                    linear_rgb const& white);

            #include "../util/const-helper.macro.hxx"
            CONST_HELPER(
                template <class F>
                void each(F&& f) CONST
                {
                    auto w = size_.x;
                    int i = 0;
                    for (auto& pixel: pixels) {
                        f(pixel, position_type{i % w, i / w});
                        i++;
                    }
                }
            )

            CONST_HELPER(
                auto& get(position_type const& pos) CONST
                {
                    auto i = index_from_bounded_pos(wrap_around(pos));
                    return pixels[i];
                }
            )

            CONST_HELPER(
                auto& operator [] (position_type const& pos) CONST
                {
                    return get(pos);
                }
            )
            #include "../util/const-helper.undef.hxx"

            color_type const* data() const { return pixels.data(); }

        private:
            dimension_type size_;
            std::vector<color_type> pixels;

            void put_bounded(position_type const& pos, color_type color)
            {
                // assert(in_bounds(pos));
                pixels[index_from_bounded_pos(pos)] = std::move(color);
            }

            auto index_from_bounded_pos(position_type const& pos) const
            {
                return pos.y * size_.x + pos.x;
            }

            auto in_bounds(position_type const& pos) const
            {
                return (all(greaterThanEqual(pos, position_type{0})) && all(lessThan(pos, size_)));
            }

            auto wrap_around(position_type const& pos) const
            {
                return (pos % size_ + size_) % size_;
            }

            friend auto load(util::as_czstring filename) -> image<linear_rgb>;
        };

        auto load(util::as_czstring filename) -> image<linear_rgb>;
        auto sample_bilinear(image<linear_rgb> const& img, glm::vec2 p) -> linear_rgb;

        extern template struct image<linear_rgb>;
        extern template struct image<srgb>;
    }

    using image_impl::image;
    using image_impl::load;
    using image_rgb = image<color::linear_rgb>;
}

