#pragma once
#include "../util/tile.hh"
#include "../image/image.hh"
#include "../lib/glm/mat3.hh"
#include "../lib/glm/vec2.hh"
#include "polar-path.hh"
#include <vector>

namespace rt::morpha
{
    struct morphing_cache_entry
    {
        glm::mat3 to_src;
        glm::mat3 to_dst;
        glm::mat3 interpolated_to_line_space;
        float interpolated_length;

        morphing_cache_entry(glm::mat3 to_src, glm::mat3 to_dst, glm::mat3 interpolated_to_line_space, float interpolated_length)
            : to_src{to_src}
            , to_dst{to_dst}
            , interpolated_to_line_space{interpolated_to_line_space}
            , interpolated_length{interpolated_length}
        {}
    };
    using morphing_cache = std::vector<morphing_cache_entry>;

    void extend_morphing_cache(polar_path_cache const& src, polar_path_cache const& interpolated, polar_path_cache const& dst, morphing_cache& cache);
    auto morph(image::image_rgb const& src, image::image_rgb const& dst, morphing_cache const& cache, float amount, util::tile tile) -> image::image_rgb;
}

