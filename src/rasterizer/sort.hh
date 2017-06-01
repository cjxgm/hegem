#pragma once
#include "../scene/scene.hh"
#include "../scene/material.hh"
#include "../scene/shape.hh"
#include "../scene/lamp.hh"
#include <deque>

namespace rt::rasterizer
{
    namespace sort_details
    {
        using scene::scene_type;
        using scene::material_id_type;
        namespace materials = scene::materials;
        namespace shapes = scene::shapes;
        namespace lamps = scene::lamps;

        template <class Shape>
        struct with_material
        {
            using shape_type = Shape;

            with_material(shape_type shape, material_id_type id)
                : shape{shape}, material_id{id} {}

            shape_type shape;
            material_id_type material_id;
        };

        struct sorted_geometry
        {
            materials::solid_color sky;
            std::deque<materials::physically_based> materials;

            std::deque<with_material<shapes::sphere>> spheres;
            std::deque<with_material<shapes::plane>> planes;
            std::deque<with_material<shapes::line_segment>> line_segments;
            std::deque<with_material<shapes::mesh>> meshes;

            std::deque<lamps::sun> sun_lamps;
            std::deque<lamps::omni> omni_lamps;
        };

        sorted_geometry sort_geometry(scene_type const& scene);
    }

    template <class Shape>
    using geometry_with_material = sort_details::with_material<Shape>;

    using sort_details::sorted_geometry;
    using sort_details::sort_geometry;
}

