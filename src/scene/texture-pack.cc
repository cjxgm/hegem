#include "../lib/glm/vec3.hh"
#include "../lib/glm/op/common.hh"
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
                auto q = glm::floor(pos / tpack.size + glm::vec3{0.01});
                float a = (int(q.x + q.y + q.z) % 2 + 2) % 2;
                return glm::mix(albedo, tpack.accent, a);
            });
    }
}

