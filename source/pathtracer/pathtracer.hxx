#pragma once
#include "../image/image.hxx"
#include "../scene/scene.hxx"
#include "../scene/view.hxx"
#include "../util/tile.hxx"
#include <functional>

namespace rt::pathtracer
{
    namespace pathtracer_details
    {
        using image_type = image::image_rgb;
        using scene::scene_type;
        using scene::view_type;
        using update_fn = std::function<void (image_type const& result_per_sample)>;

        auto pathtrace(scene_type const& scene, view_type const& view, util::tile const& tile, update_fn update={}) -> image_type;
    }

    using pathtracer_details::pathtrace;
}

