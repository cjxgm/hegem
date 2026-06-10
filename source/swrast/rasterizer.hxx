#pragma once
#include "../scene/scene.hxx"
#include "../image/image.hxx"

namespace rt::swrast
{
    using image_type = image::image_rgb;
    auto rasterize(scene::scene_type const& s, scene::view_type view) -> image_type;
}

