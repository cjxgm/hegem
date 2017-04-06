#include "../../scene/material.hh"
#include "../../scene/lamp.hh"
#include "../shade.hh"
#include <stdexcept>

namespace rt::raytracer::shading_details
{
    namespace
    {
        namespace materials = scene::materials;
        namespace lamps = scene::lamps;

        struct environment_shader
        {
            scene_type const& scene;
            ray_type const& ray;

            template <class Material>
            auto operator () (Material const& mat) const
            {
                return impl(mat);
            }

        private:
            color_type impl(materials::solid_color const& mat) const
            {
                return mat.color;
            }

            color_type impl(materials::phong const& mat) const
            {
                throw std::runtime_error{"Phong materials cannot be used by environment."};
            }

            color_type impl(materials::physically_based const& mat) const
            {
                throw std::runtime_error{"PBR materials cannot be used by environment."};
            }
        };
    }

    color_type shade_environment(scene_type const& scene, ray_type const& ray)
    {
        environment_shader shader{scene, ray};
        return apply_visitor(shader, scene.materials[scene.environment]);
    }
}

