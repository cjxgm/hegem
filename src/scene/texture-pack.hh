#pragma once
#include "../lib/mapbox/variant.hh"
#include "../image/color.hh"

namespace rt::scene
{
    namespace texture_packs
    {
        using color_type = image::color::linear_rgb;

        struct pure
        {
        };

        struct checkerboard
        {
            color_type accent;
            float size;
        };

        struct noise_fbm
        {
            color_type albedo_accent;
            int albedo_details;
            float albedo_size;
            glm::vec3 albedo_seed;
            glm::vec2 albedo_range;

            float roughness_accent;
            int roughness_details;
            float roughness_size;
            glm::vec3 roughness_seed;
            glm::vec2 roughness_range;

            float density_accent;
            int density_details;
            float density_size;
            glm::vec3 density_seed;
            glm::vec2 density_range;
        };

        using texture_pack_type = mapbox::util::variant<
            checkerboard,
            noise_fbm,
            pure
        >;

        color_type sample_albedo(texture_pack_type const& tpack, color_type const& albedo, glm::vec3 const& pos);
        float sample_roughness(texture_pack_type const& tpack, float roughness, glm::vec3 const& pos);
        float sample_density(texture_pack_type const& tpack, float density, glm::vec3 const& pos);
    }

    using texture_packs::texture_pack_type;
}

