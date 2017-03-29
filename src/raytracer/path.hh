#pragma once
#include "../image/image.hh"
#include "../utils/constraints.hh"
#include "hit.hh"
#include <vector>

namespace rt::raytracer
{
    // A path is a consecutive sequence of object hits
    // arranged in descending bouncing count order.
    //
    // That is,
    //
    //   - 0th bounce is `*hits.rbegin()` i.e. `hits.back()`
    //   - 1st bounce is `*next(hits.rbegin(), 1)`
    //   - 2nd bounce is `*next(hits.rbegin(), 2)`
    //   - 3rd bounce is `*next(hits.rbegin(), 3)`
    //   - ⋮
    //   - last bounce is `*hits.begin()` i.e. `hits.front()`
    struct path_type: utils::only_movable
    {
        std::vector<hits::object> hits;
    };

    using path_buffer_type = image::image<path_type>;
}

namespace rt::image::image_impl
{
    extern template struct image<raytracer::path_type>;
}

