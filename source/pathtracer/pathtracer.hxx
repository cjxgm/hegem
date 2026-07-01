#pragma once
#include "../image/image.hxx"
#include "../scene/scene.hxx"
#include "../scene/view.hxx"
#include "../tool/tile.hxx"
#include <memory>
#include <atomic>
#include <functional>

namespace hegem::pathtracer
{
    namespace pathtracer_details
    {
        using image_type = image::image_rgb;
        using scene::scene_type;
        using scene::view_type;
        using update_fn = std::function<void (image_type const& result_per_sample)>;
        using shared_canceled_type = std::shared_ptr<std::atomic_bool>;

        auto pathtrace(shared_canceled_type shared_canceled, scene_type const& scene, view_type const& view, tool::tile const& tile, update_fn update={}) -> image_type;
    }

    using pathtracer_details::pathtrace;
}

