#include "../../lib/glm/op/common.hxx"
#include "../../lib/glm/op/geom.hxx"
#include "../../scene/material.hxx"
#include "../../scene/lamp.hxx"
#include "../shade.hxx"
#include <stdexcept>

namespace rt::raytracer::shading_details
{
    namespace
    {
        namespace materials = scene::materials;
        namespace lamps = scene::lamps;

        const auto ground_dir = glm::vec3{0.0f, -1.0f, 0.0f};
        const auto ground_color = glm::vec3{0.0f, 0.0f, 0.0f};

        struct environment_shader
        {
            scene_type const& scene;
            ray_type const& viewing;

            template <class Material>
            auto operator () (Material const& mat) const
            {
                return impl(mat);
            }

        private:
            color_type impl(materials::solid_color const& mat) const
            {
                auto color = mat.color;

                for (auto& lamp: scene.lamps) {
                    lamp.match(
                            [&] (lamps::sun const& lamp) {
                                auto d = glm::max(-dot(*viewing.dir, *lamp.dir), 0.0f);
                                auto strength = glm::pow(d, 10.0f) * 0.1f
                                        + glm::pow(d, 50.0f) * 0.5f
                                        + glm::pow(d, 600.0f) * 3.0f
                                        + glm::smoothstep(0.4f, 0.5f, glm::pow(d, 500.0f)) * 3.0f;
                                color += lamp.color * strength;
                            },
                            [] (auto&) {});
                }

                float groundness = glm::max(dot(*viewing.dir, ground_dir), 0.0f);
                color = glm::mix(color, ground_color, groundness);

                return color;
            }

            color_type impl(materials::phong const&) const
            {
                throw std::runtime_error{"Phong materials cannot be used by environment."};
            }

            color_type impl(materials::physically_based const&) const
            {
                throw std::runtime_error{"PBR materials cannot be used by environment."};
            }
        };
    }

    color_type shade_environment(scene_type const& scene, ray_type const& viewing)
    {
        environment_shader shader{scene, viewing};
        return apply_visitor(shader, scene.materials[scene.environment]);
    }
}

