#pragma once
#include "../lib/glm/mat4.hh"
#include "../scene/scene.hh"
#include "../scene/material.hh"
#include "../scene/shape.hh"
#include "../scene/lamp.hh"
#include "../kul/system.hh"
#include "../glu/resource.hh"
#include "../math/direction.hh"
#include <vector>

namespace rt::rasterizer
{
    namespace sort_details
    {
        using scene::scene_type;
        using scene::material_id_type;
        using math::direction_type;
        namespace materials = scene::materials;
        namespace shapes = scene::shapes;
        namespace lamps = scene::lamps;

        template <class Shape>
        struct with_material_xform
        {
            using shape_type = Shape;

            with_material_xform(
                shape_type shape,
                material_id_type id,
                glm::mat4 model_to_world,
                glm::mat4 world_to_model)
                : shape{shape}
                , material_id{id}
                , model_to_world{model_to_world}
                , world_to_model{world_to_model}
            {}

            shape_type shape;
            material_id_type material_id;
            glm::mat4 model_to_world;
            glm::mat4 world_to_model;
        };

        struct compiled_spark
        {
            glu::shared_program prog_spark;
            int num_particles;
        };

        struct uploaded_mesh
        {
            glu::shared_buffer vertices_buffer;
            glu::shared_buffer elements_buffer;
            glu::shared_vertex_array vao;
            int element_count;
        };

        struct sorted_geometry
        {
            materials::solid_color sky;
            std::vector<materials::physically_based> materials;

            std::vector<with_material_xform<shapes::sphere>> spheres;
            std::vector<with_material_xform<shapes::plane>> planes;
            std::vector<with_material_xform<shapes::voxel>> voxels;
            std::vector<with_material_xform<compiled_spark>> sparks;
            std::vector<with_material_xform<uploaded_mesh>> meshes;

            struct
            {
                std::vector<direction_type> dirs;
                std::vector<lamps::color_type> colors;
            }
            sun_lamp;

            struct
            {
                std::vector<lamps::position_type> centers;
                std::vector<lamps::color_type> colors;
            }
            omni_lamp;
        };

        sorted_geometry sort_geometry(scene_type const& scene);
    }

    template <class Shape>
    using geometry_with_material_and_xform = sort_details::with_material_xform<Shape>;

    using sort_details::sorted_geometry;
    using sort_details::sort_geometry;
}

