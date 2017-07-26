#include "../lib/glm/vec3.hh"
#include "texture-pack.hh"

namespace rt::scene::texture_packs
{
    color_type sample_albedo(texture_pack_type const& tpack, color_type const& albedo, glm::vec3 const& pos)
    {
        return tpack.match(
            [&] (texture_packs::pure const& /*tpack*/) {
                return albedo;
            },
            [&] (texture_packs::checkerboard const& tpack) {
                // TODO
                (void)pos;
                return albedo;
            });
    }
}

