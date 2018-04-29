#include "../lib/glm/op/common.hh"
#include "morph.hh"

namespace rt::morpha
{
    auto morph(image::image_rgb const& a, image::image_rgb const& b, float amount, util::tile tile) -> image::image_rgb
    {
        image::image_rgb result{{tile.w, tile.h}};
        result.each([&] (auto& color, auto pos) {
            pos.x += tile.x;
            pos.y += tile.y;
            auto c0 = a[pos];
            auto c1 = b[pos];
            color = glm::mix(c0, c1, amount);
        });
        return result;
    }
}

