#include "image.hh"
#include <algorithm>
#include <fstream>
#include <cassert>

#include "../lib/stb/image-write.inl"

namespace rt::image::image_impl
{
    template struct image<linear_rgb>;
    template struct image<srgb>;

    image<srgb> to_srgb(image<linear_rgb> const& src)
    {
        image<srgb> dst{src.size()};
        std::transform(
                begin(src.pixels), end(src.pixels),
                begin(dst.pixels),
                [] (auto& linear_rgb) { return color::to_srgb(linear_rgb); });
        return dst;
    }

    void write(image<srgb> const& src, gsl::cstring_span<> output_path)
    {
        std::ofstream file{ensure_z(output_path).data()};
        auto size = src.size();
        stbi_write_png_to_func(
                [] (void* context, void* data, int size) {
                    auto& file = *reinterpret_cast<std::ofstream*>(context);
                    file.write(reinterpret_cast<char*>(data), size);
                },
                &file,
                size.x, size.y, 3,
                src.pixels.data(),
                0);
    }

    image<linear_rgb> half(image<linear_rgb> const& src)
    {
        using position_type = image<linear_rgb>::position_type;
        using dimension_type = image<linear_rgb>::dimension_type;

        auto size = src.size();
        assert(size.x % 2 == 0);
        assert(size.y % 2 == 0);

        image<linear_rgb> dst{size / dimension_type{2}};

        src.each([&] (auto& pixel, auto pos) {
            int i = dst.index_from_bounded_pos(pos / position_type{2});
            dst.pixels[i] += pixel / linear_rgb{4};
        });

        return dst;
    }
}

