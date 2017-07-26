#pragma once
#include "../lib/mapbox/variant.hh"
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
        };

        struct physically_based
        {
            texture_pack_type texture_pack;
            color_type albedo;
            color_type reflection;
            float roughness;
            float ior;
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
    }

    using materials::material_id_type;
    using materials::material_type;
}

