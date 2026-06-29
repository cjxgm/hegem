#include "../lib/glm/vec3.hxx"
#include "../lib/glm/op/common.hxx"
#include "image.hxx"
#include <algorithm>
#include <fstream>
#include <cassert>
#include <random>
#include <memory>

#include "../lib/stb/image-write.inl"
#include "../lib/stb/image-read.inl"

namespace hegem::image::image_impl
{
    template struct image<linear_rgb>;
    template struct image<srgb>;

    image<srgb> to_srgb(image<linear_rgb> const& src)
    {
        std::random_device rd;
        std::mt19937 gen{rd()};
        std::uniform_real_distribution<float> dis{-0.5 / 255.0, 0.5 / 255.0};

        image<srgb> dst{src.size()};
        std::transform(
                begin(src.pixels), end(src.pixels),
                begin(dst.pixels),
                [&] (auto& linear) {
                    linear_rgb dither_noise{dis(gen), dis(gen), dis(gen)};
                    return color::to_srgb(linear + dither_noise);
                });
        return dst;
    }

    auto write(image<srgb> const& src, tool::as_czstring output_path) -> void
    {
        std::ofstream file{output_path.data()};
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

    image<linear_rgb> tonemap(
            image<linear_rgb> const& src,
            linear_rgb const& black, linear_rgb const& white)
    {
        image<linear_rgb> dst{src.size()};

        dst.each([&] (auto& pixel, auto pos) {
            pixel = color::tonemap(src[pos], black, white);
        });

        return dst;
    }

    inline namespace
    {
        struct stb_image_deleter
        {
            auto operator () (void* p) -> void
            {
                if (p) stbi_image_free(p);
            }
        };

        using stb_image_buffer = std::unique_ptr<float, stb_image_deleter>;
    }

    auto load(tool::as_czstring filename) -> image<linear_rgb>
    {
        int w;
        int h;
        auto buf = stb_image_buffer{stbi_loadf(filename, &w, &h, nullptr, 3)};
        if (!buf) {
            throw std::runtime_error{
                std::string{"Failed to load image: "} +
                stbi_failure_reason()
            };
        }

        image<linear_rgb> img{{w, h}};
        auto p = reinterpret_cast<linear_rgb*>(buf.get());
        std::transform(
            p, p + img.pixels.size(),
            img.pixels.begin(),
            color::gamma_to_display
        );
        return img;
    }

    auto sample_bilinear(image<linear_rgb> const& img, glm::vec2 p) -> linear_rgb
    {
        p = glm::clamp(p, glm::vec2{0.0f}, glm::vec2{img.size() - 1});
        auto p00 = glm::ivec2{p};
        auto p01 = p00 + glm::ivec2{0, 1};
        auto p10 = p00 + glm::ivec2{1, 0};
        auto p11 = p00 + glm::ivec2{1, 1};

        auto s00 = img[p00];
        auto s01 = img[p01];
        auto s10 = img[p10];
        auto s11 = img[p11];

        auto f = glm::fract(p);
        auto s0 = glm::mix(s00, s01, f.y);
        auto s1 = glm::mix(s10, s11, f.y);
        auto s = glm::mix(s0, s1, f.x);
        return s;
    }
}

