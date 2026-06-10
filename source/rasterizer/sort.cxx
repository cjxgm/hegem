#include "sort.hxx"
#include "../lib/gl/gl.hxx"
#include "../scene/node.hxx"
#include "../util/journal.hxx"
#include "../glu/shader.hxx"
#include <string>
#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace rt::rasterizer::sort_details
{
    namespace
    {
        using scene::material_type;
        namespace nodes = scene::nodes;
        namespace texture_packs = scene::texture_packs;

        static constexpr auto sun_lamp_capacity = 32;
        static constexpr auto omni_lamp_capacity = 32;

        auto j() { return rt::util::journal{"RAST"}; }

        namespace sort_materials
        {
            materials::physically_based coerce_material_to_pbr(material_type m)
            {
                return m.match(
                    [] (materials::solid_color m) {
                        return materials::physically_based{
                            texture_packs::pure{},
                            m.color,
                            {},
                            1.0f,
                            1.5f,
                        };
                    },
                    [] (materials::phong m) {
                        // FIXME: find a better coercion for Phong -> PBR
                        return materials::physically_based{
                            texture_packs::pure{},
                            m.diffuse,
                            m.reflection,
                            m.roughness,
                            m.ior,
                        };
                    },
                    [] (materials::physically_based m) { return m; });
            }

            void sort(scene_type const& scene, sorted_geometry& sg)
            {
                sg.sky = scene.materials[scene.environment].template get<materials::solid_color>();
                std::transform(
                    begin(scene.materials), end(scene.materials),
                    std::back_inserter(sg.materials),
                    coerce_material_to_pbr);
            }
        }

        namespace sort_lamps
        {
            struct lamp_sorter
            {
                sorted_geometry& sg;

                void operator () (lamps::sun lamp)
                {
                    if (sg.sun_lamp.colors.size() == sun_lamp_capacity) {
                        j() << "WARNING: sun lamp amount is capped at " << sun_lamp_capacity << "\n";
                        return;
                    }
                    sg.sun_lamp.dirs.emplace_back(lamp.dir);
                    sg.sun_lamp.colors.emplace_back(lamp.color);
                }

                void operator () (lamps::omni lamp)
                {
                    if (sg.omni_lamp.colors.size() == omni_lamp_capacity) {
                        j() << "WARNING: omni lamp amount is capped at " << omni_lamp_capacity << "\n";
                        return;
                    }
                    sg.omni_lamp.centers.emplace_back(lamp.center);
                    sg.omni_lamp.colors.emplace_back(lamp.color);
                }
            };

            void sort(scene_type const& scene, sorted_geometry& sg)
            {
                lamp_sorter sorter{sg};
                for (auto lamp: scene.lamps)
                    apply_visitor(sorter, lamp);
            }
        }

        namespace sort_objects
        {
            struct shape_sorter
            {
                sorted_geometry& sg;
                material_id_type material_id;
                glm::mat4 const& model_to_world;
                glm::mat4 const& world_to_model;

                void operator () (shapes::sphere shape)
                {
                    sg.spheres.emplace_back(shape, material_id, model_to_world, world_to_model);
                }

                void operator () (shapes::plane shape)
                {
                    sg.planes.emplace_back(shape, material_id, model_to_world, world_to_model);
                }

                void operator () (shapes::voxel shape)
                {
                    sg.voxels.emplace_back(shape, material_id, model_to_world, world_to_model);
                }

                void operator () (shapes::hemesh const& /*shape*/)
                {
                    throw std::logic_error{"hemeshes should have been converted into meshes."};
                }

                void operator () (shapes::spark_system const& shape)
                {
                    auto params = std::string{};
                    params += "#define KUL_SPARK\n";

                    auto add_param = [&] (char const* name, auto& timeline) {
                        params += "#define ";
                        params += name;
                        params += " ";
                        params += timeline.expression.apply("kul_time");
                        params += "\n";
                    };
                    add_param("KUL_POS_X", shape.pos_x);
                    add_param("KUL_POS_Y", shape.pos_y);
                    add_param("KUL_POS_Z", shape.pos_z);
                    add_param("KUL_RADIUS", shape.radius);
                    add_param("KUL_EMITTING_COLOR_R", shape.emitting_color_r);
                    add_param("KUL_EMITTING_COLOR_G", shape.emitting_color_g);
                    add_param("KUL_EMITTING_COLOR_B", shape.emitting_color_b);
                    add_param("KUL_OPACITY", shape.opacity);

                    auto prog = glu::shader_factory::program_from_name("spark", params);

                    sg.sparks.emplace_back(compiled_spark{
                        prog,
                        shape.num_particles,
                    }, material_id, model_to_world, world_to_model);
                }

                void operator () (shapes::mesh shape)
                {
                    auto& vao_pool = glu::vertex_array_pool::instance();
                    auto& buf_pool = glu::buffer_pool::instance();
                    auto vao = vao_pool.allocate();
                    auto vertices_buffer = buf_pool.allocate();
                    auto elements_buffer = buf_pool.allocate();

                    gl::enable_vertex_array_attrib(vao, 0);  // vec3 pos
                    gl::enable_vertex_array_attrib(vao, 1);  // vec3 normal
                    gl::vertex_array_attrib_format(
                        vao, 0,
                        3, gl::float_, false,
                        0);
                    gl::vertex_array_attrib_format(
                        vao, 1,
                        3, gl::float_, false,
                        0);
                    gl::vertex_array_vertex_buffer(vao, 0, vertices_buffer, 0,                 sizeof(shapes::vert_attributes));
                    gl::vertex_array_vertex_buffer(vao, 1, vertices_buffer, sizeof(glm::vec3), sizeof(shapes::vert_attributes));
                    gl::vertex_array_element_buffer(vao, elements_buffer);

                    gl::named_buffer_data(
                        vertices_buffer,
                        sizeof(decltype(shape.verts)::value_type) * shape.verts.size(),
                        shape.verts.data(),
                        gl::static_draw);
                    gl::named_buffer_data(
                        elements_buffer,
                        sizeof(decltype(shape.faces)::value_type) * shape.faces.size(),
                        shape.faces.data(),
                        gl::static_draw);

                    sg.meshes.emplace_back(uploaded_mesh{
                        vertices_buffer,
                        elements_buffer,
                        vao,
                        static_cast<int>(shape.faces.size()) * 3,
                    }, material_id, model_to_world, world_to_model);
                }
            };

            void sort(scene_type const& scene, sorted_geometry& sg)
            {
                for (auto& obj: scene.cache.objects) {
                    shape_sorter sorter{sg, obj.material_id, obj.model_to_world, obj.world_to_model};
                    apply_visitor(sorter, obj.shape);
                }
            }
        }
    }

    sorted_geometry sort_geometry(scene_type const& scene)
    {
        sorted_geometry sg;
        sort_materials::sort(scene, sg);
        sort_objects::sort(scene, sg);
        sort_lamps::sort(scene, sg);
        return sg;
    }
}

