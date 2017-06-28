#include "sort.hh"
#include "../scene/node.hh"
#include "../util/journal.hh"
#include <algorithm>
#include <iterator>

namespace rt::rasterizer::sort_details
{
    namespace
    {
        using scene::material_type;
        namespace nodes = scene::nodes;

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
                            .albedo = m.color,
                            .reflection = glm::vec3{0},
                            .roughness = 1,
                            .ior = 1.5,
                        };
                    },
                    [] (materials::phong m) {
                        // FIXME: find a better coercion for Phong -> PBR
                        return materials::physically_based{
                            .albedo = m.diffuse,
                            .reflection = m.reflection,
                            .roughness = 0,
                            .ior = m.ior,
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

        namespace sort_nodes
        {
            struct shape_sorter
            {
                sorted_geometry& sg;
                material_id_type material_id;

                void operator () (shapes::sphere shape) { sg.spheres.emplace_back(shape, material_id); }
                void operator () (shapes::plane shape) { sg.planes.emplace_back(shape, material_id); }
                void operator () (shapes::mesh shape) { sg.meshes.emplace_back(shape, material_id); }
            };

            struct node_sorter
            {
                sorted_geometry& sg;

                void operator () (nodes::object const& node)
                {
                    shape_sorter sorter{sg, node.material_id};
                    apply_visitor(sorter, node.shape);
                }

                void operator () (nodes::group const& group)
                {
                    for (auto& node: group.nodes) {
                        node_sorter sorter{sg};
                        apply_visitor(sorter, node);
                    }
                }
            };

            void sort(scene_type const& scene, sorted_geometry& sg)
            {
                node_sorter sorter{sg};
                apply_visitor(sorter, scene.root);
            }
        }
    }

    sorted_geometry sort_geometry(scene_type const& scene)
    {
        sorted_geometry sg;
        sort_materials::sort(scene, sg);
        sort_nodes::sort(scene, sg);
        sort_lamps::sort(scene, sg);
        return sg;
    }
}

