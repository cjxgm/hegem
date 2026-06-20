#include "../lib/glm/op/common.hxx"
#include "../lib/glm/op/trig.hxx"
#include "../lib/glm/vec3.hxx"
#include "morph.hxx"
#include <stdexcept>
#include <utility>      // for std::pair

namespace hegem::silo
{
    namespace
    {
        auto line_space_to_image_space(glm::vec2 origin, float y_angle, float y_scale) -> glm::mat3
        {
            auto c = glm::cos(y_angle);
            auto s = glm::sin(y_angle);
            auto y = glm::vec3{c, s, 0.0f} * y_scale;
            auto x = glm::vec3{s,-c, 0.0f};
            auto t = glm::vec3{origin, 1.0f};
            return { x, y, t };
        }

        auto weight_of(morphing_cache_entry const& entry, glm::vec2 interpolated_pos, float smoothness, float decay, float length_influence) -> float
        {
            auto length = entry.interpolated_length;
            auto line_space_pos = glm::vec2{entry.interpolated_to_line_space * glm::vec3{interpolated_pos, 1.0f}};
            auto y_half = glm::vec2{0.0f, 0.5f};
            auto per_axis_distance = glm::max(glm::abs(line_space_pos - y_half) - y_half, glm::vec2{0.0f, 0.0f});
            per_axis_distance.y *= length;
            auto dist_to_line_segment = glm::length(per_axis_distance);
            return glm::pow(glm::pow(length, length_influence) / (dist_to_line_segment + smoothness), decay);
        }

        // returns (pos_in_src, pos_in_dst)
        auto warp(morphing_cache const& cache, glm::vec2 pos, float smoothness, float decay, float length_influence) -> std::pair<glm::vec2, glm::vec2>
        {
            if (cache.empty()) return { pos, pos };

            auto src_pos_sum = glm::vec2{};
            auto dst_pos_sum = glm::vec2{};
            auto weight_sum = 0.0f;

            for (auto& entry: cache) {
                auto weight = weight_of(entry, pos, smoothness, decay, length_influence);
                auto weighted_pos = glm::vec3{pos, 1.0f} * weight;
                src_pos_sum += glm::vec2{entry.to_src * weighted_pos};
                dst_pos_sum += glm::vec2{entry.to_dst * weighted_pos};
                weight_sum += weight;
            }

            return {
                src_pos_sum / weight_sum,
                dst_pos_sum / weight_sum,
            };
        }
    }

    auto extend_morphing_cache(polar_path_cache const& src, polar_path_cache const& interpolated, polar_path_cache const& dst, morphing_cache& cache) -> void
    {
        if (src.size() != dst.size() || src.size() != interpolated.size())
            throw std::logic_error{"cache must have same size"};

        if (src.size() < 2) return;
        auto it0 = ++src.begin();
        auto it1 = ++dst.begin();
        auto it2 = ++interpolated.begin();
        auto last0 = src.end();

        for (; it0<last0; ++it0, ++it1, ++it2) {
            auto          src_length = glm::length(it0->pos - (it0-1)->pos);
            auto          dst_length = glm::length(it1->pos - (it1-1)->pos);
            auto interpolated_length = glm::length(it2->pos - (it2-1)->pos);
            auto          src_from_line_space = line_space_to_image_space((it0-1)->pos, it0->angle_sum_so_far, src_length);
            auto          dst_from_line_space = line_space_to_image_space((it1-1)->pos, it1->angle_sum_so_far, dst_length);
            auto interpolated_from_line_space = line_space_to_image_space((it2-1)->pos, it2->angle_sum_so_far, interpolated_length);
            auto interpolated_to_line_space = glm::inverse(interpolated_from_line_space);

            auto to_src = src_from_line_space * interpolated_to_line_space;
            auto to_dst = dst_from_line_space * interpolated_to_line_space;
            cache.emplace_back(to_src, to_dst, interpolated_to_line_space, interpolated_length);
        }
    }

    auto morph(int lower_quality, image::image_rgb const& src, image::image_rgb const& dst, morphing_cache const& cache, float amount, tool::tile tile, float smoothness, float decay, float length_influence) -> image::image_rgb
    {
        using sample_fn = auto (image::image_rgb const& img, glm::vec2 pos) -> image::color::linear_rgb;
        sample_fn* sample;
        int scaling;
        if (lower_quality < 1) {
            sample = [] (auto& img, auto pos) { return sample_bilinear(img, pos); };
            scaling = 1;
        } else {
            sample = [] (auto& img, auto pos) { return img[glm::clamp(glm::round(pos), glm::vec2{0.0f}, glm::vec2{img.size() - 1})]; };
            scaling = 1 << (lower_quality - 1);
        }

        image::image_rgb result{{tile.w, tile.h}};
        result.each([&] (auto& color, auto pos) {
            if (pos.x % scaling == 0 && pos.y % scaling == 0) {
                pos.x += tile.x;
                pos.y += tile.y;
                auto [src_pos, dst_pos] = warp(cache, pos, smoothness, decay, length_influence);
                auto c0 = sample(src, src_pos);
                auto c1 = sample(dst, dst_pos);
                color = glm::mix(c0, c1, amount);
            } else {
                pos.x = pos.x / scaling * scaling;
                pos.y = pos.y / scaling * scaling;
                color = result[pos];
            }
        });

        return result;
    }
}

