#include "../lib/glm/vec2.hh"
#include "../lib/glm/vec3.hh"
#include "../lib/glm/op/common.hh"
#include "../math/noise.hh"
#include "texture-pack.hh"

namespace rt::scene::texture_packs
{
    color_type sample_albedo(texture_pack_type const& tpack, color_type const& albedo, glm::vec3 const& pos)
    {
        return tpack.match(
            [&] (texture_packs::checkerboard const& tpack) {
                auto q = glm::floor(pos / tpack.size + glm::vec3{0.01});
                float a = (int(q.x + q.y + q.z) % 2 + 2) % 2;
                return glm::mix(albedo, tpack.accent, a);
            },
            [&] (texture_packs::noise_fbm const& tpack) {
                auto t = math::fbm_noise(
                    pos,
                    tpack.albedo_details,
                    tpack.albedo_size,
                    tpack.albedo_range,
                    tpack.albedo_seed
                );
                return glm::mix(albedo, tpack.albedo_accent, t);
            },
            [&] (texture_packs::pure const& tpack) {
                return albedo;
            });
    }

    float sample_roughness(texture_pack_type const& tpack, float roughness, glm::vec3 const& pos)
    {
        return tpack.match(
            [&] (texture_packs::checkerboard const& tpack) {
                return roughness;
            },
            [&] (texture_packs::noise_fbm const& tpack) {
                auto t = math::fbm_noise(
                    pos,
                    tpack.roughness_details,
                    tpack.roughness_size,
                    tpack.roughness_range,
                    tpack.roughness_seed
                );
                return glm::mix(roughness, tpack.roughness_accent, t);
            },
            [&] (texture_packs::pure const& tpack) {
                return roughness;
            });
    }

    float sample_density(texture_pack_type const& tpack, float density, glm::vec3 const& pos)
    {
        return tpack.match(
            [&] (texture_packs::checkerboard const& tpack) {
                return density;
            },
            [&] (texture_packs::noise_fbm const& tpack) {
                auto t = math::fbm_noise(
                    pos,
                    tpack.density_details,
                    tpack.density_size,
                    tpack.density_range,
                    tpack.density_seed
                );
                return glm::mix(density, tpack.density_accent, t);
            },
            [&] (texture_packs::pure const& tpack) {
                return density;
            });
    }
}

