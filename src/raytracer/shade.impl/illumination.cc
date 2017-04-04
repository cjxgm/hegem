#include "../../scene/material.hh"
#include "../shade.hh"
#include <stdexcept>

namespace rt::raytracer::shading_details
{
    namespace
    {
        namespace materials = scene::materials;

        struct illumination_shader
        {
            hits::object const& hit;
            color_type const& diffuse;
            color_type const& reflected;
            color_type const& refracted;

            template <class Material>
            auto operator () (Material const& mat) const
            {
                return impl(mat);
            }

        private:
            color_type impl(materials::solid_color const& mat) const
            {
                return diffuse;
            }

            color_type impl(materials::phong const& mat) const
            {
                // TODO
                return mat.diffuse;
            }

            color_type impl(materials::physically_based const& mat) const
            {
                // TODO
                return diffuse;
            }
        };
    }

    color_type shade_illumination(
            scene_type const& scene,
            hits::object const& hit,
            color_type const& diffuse,
            color_type const& reflected,
            color_type const& refracted)
    {
        illumination_shader shader{hit, diffuse, reflected, refracted};
        return scene.materials[hit.material_id].match(shader);
    }
}

