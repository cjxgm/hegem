#pragma once
#include "lib/gsl/string-span.hh"
#include "color.hh"
#include <vector>

#define WANT_VEC2
#include "lib/glm.hh"
#undef WANT_VEC2

namespace ifs
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

            image(dimension_type size, color_type fill={})
                : size_{size}
                , pixels(size.x*size.y, fill)
            {}

            void put_repeat(position_type const& pos, color_type color)
            {
                put_bounded(wrap_around(pos), color);
            }

            void put_clamp(position_type const& pos, color_type color)
            {
                if (in_bounds(pos)) put_bounded(pos, color);
            }

            auto& size() const { return size_; }

            friend image<srgb> to_srgb(image<linear_rgb> const& src);
            friend void write(image<srgb> const& src, gsl::cstring_span<> output_path);
            friend image<linear_rgb> half(image<linear_rgb> const& src);

        private:
            dimension_type size_;
            std::vector<color_type> pixels;

            void put_bounded(position_type const& pos, color_type color)
            {
                // assert(in_bounds(pos));
                pixels[index_from_bounded_pos(pos)] = color;
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
        };

        extern template struct image<linear_rgb>;
        extern template struct image<srgb>;
    }

    using image_impl::image;
    using image_rgb = image<color::linear_rgb>;
}

