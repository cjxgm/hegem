#include "sort.hh"
#include <algorithm>
#include <iterator>

namespace rt::rasterizer::sort_details
{
    namespace
    {
        using scene::material_type;

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

                void operator () (lamps::sun  lamp) { sg.sun_lamps .emplace_back(lamp); }
                void operator () (lamps::omni lamp) { sg.omni_lamps.emplace_back(lamp); }
            };

            void sort(scene_type const& scene, sorted_geometry& sg)
            {
                lamp_sorter sorter{sg};
                for (auto lamp: scene.lamps)
                    apply_visitor(sorter, lamp);
            }
        }
    }

    sorted_geometry sort_geometry(scene_type const& scene)
    {
        sorted_geometry sg;
        sort_materials::sort(scene, sg);
        // TODO: sort_nodes::sort(scene, sg);
        sort_lamps::sort(scene, sg);
        return sg;
    }
}

