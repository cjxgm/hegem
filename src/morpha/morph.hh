#pragma once
#include "../util/tile.hh"
#include "../image/image.hh"

namespace rt::morpha
{
    auto morph(image::image_rgb const& a, image::image_rgb const& b, float amount, util::tile tile) -> image::image_rgb;
}

