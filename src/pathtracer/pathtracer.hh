#pragma once
#include "../image/image.hh"
#include "../scene/scene.hh"
#include "../scene/view.hh"
#include "../util/tile.hh"

namespace rt::pathtracer
{
    namespace pathtracer_details
    {
        using image_type = image::image_rgb;
        using scene::scene_type;
        using scene::view_type;

        auto pathtrace(scene_type const& scene, view_type const& view, util::tile const& tile) -> image_type;
    }

    using pathtracer_details::pathtrace;
}

