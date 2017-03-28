#pragma once
#include "../lib/mapbox/variant.hh"
#include "materials/solid-color.hh"
#include "materials/phong.hh"
#include "materials/physically-based.hh"
#include <cstddef>

namespace rt::scene
{
    namespace materials
    {
        using material_type = mapbox::util::variant<
            solid_color,
            phong,
            physically_based
        >;
    }

    using materials::material_type;
    using material_id_type = std::size_t;
}

