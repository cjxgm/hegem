#pragma once
#include "../scene/scene.hh"
#include "../image/image.hh"

namespace rt::swrast
{
    using image_type = image::image_rgb;
    auto rasterize(scene::scene_type const& s, scene::view_type view) -> image_type;
}

