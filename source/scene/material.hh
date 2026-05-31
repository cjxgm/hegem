#pragma once
#include "../lib/mapbox/variant.hh"
#include "../lib/glm/op/common.hh"
#include "../image/color.hh"
#include "texture-pack.hh"
#include <cstddef>

namespace rt::scene
{
    namespace materials
    {
        using color_type = image::color::linear_rgb;

        struct solid_color
        {
            color_type color;
        };

        struct phong
        {
            color_type diffuse;
            color_type reflection;
            color_type refraction;
            color_type specular;
            float specular_exp;
            float ior;
            float roughness;
        };

        struct physically_based
        {
            texture_pack_type texture_pack;
            color_type albedo;
            color_type reflection;
            color_type emission;
            float metalness;
            float roughness;
            float ior;
            // Volumn
            float opacity;
            float density;

            physically_based(
                texture_pack_type tpack,
                color_type base,
                float metalness,
                float roughness,
                float ior)
                : physically_based{
                    std::move(tpack),
                    base,
                    color_type{0.0f},
                    metalness,
                    roughness,
                    ior,
                    1.0f,
                    0.0f,
                }
            {}

            physically_based(
                texture_pack_type tpack,
                color_type base,
                color_type emission,
                float metalness,
                float roughness,
                float ior,
                float opacity,
                float density)
                : texture_pack{std::move(tpack)}
                , albedo{glm::mix(base, color_type{0.0f}, metalness)}
                , reflection{glm::mix(color_type{1.0f}, base, metalness)}
                , emission{emission}
                , metalness{metalness}
                , roughness{roughness}
                , ior{ior}
                , opacity{opacity}
                , density{density}
            {}

            // legacy
            physically_based(
                texture_pack_type tpack,
                color_type albedo,
                color_type reflection,
                float roughness,
                float ior)
                : texture_pack{std::move(tpack)}
                , albedo{albedo}
                , reflection{reflection}
                , emission{0.0f}
                , metalness{0.0f}
                , roughness{roughness}
                , ior{ior}
                , opacity{1.0f}
                , density{0.0f}
            {}
        };

        using material_id_type = std::size_t;
        using material_type = mapbox::util::variant<
            solid_color,
            phong,
            physically_based
        >;

        bool reflective(material_type const& mat);
        bool refractive(material_type const& mat);
        float index_of_refraction(material_type const& mat);
        float roughness(material_type const& mat);
    }

    using materials::material_id_type;
    using materials::material_type;
}

