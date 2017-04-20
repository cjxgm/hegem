#include "../../lib/glm/vec3.hh"
#include "../../lib/glm/op/common.hh"
#include "../../lib/glm/op/geom.hh"
#include "../../scene/material.hh"
#include "../../math/unit.hh"
#include "../shade.hh"
#include <stdexcept>

namespace rt::raytracer::shading_details
{
    namespace
    {
        namespace materials = scene::materials;
        using direction_type = math::unit<glm::vec3>;

        float fresnel_schlick(float ior, direction_type viewing, direction_type normal)
        {
            auto base = (ior - 1) / (ior + 1);
            base *= base;
            auto exp = 1 + dot(*viewing, *normal);
            exp = exp*exp*exp*exp*exp;
            return base + (1-base)*exp;
        }

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
            color_type impl(materials::solid_color const& /*mat*/) const
            {
                return diffuse;
            }

            color_type impl(materials::phong const& mat) const
            {
                // TODO: refraction
                return diffuse + mat.reflection * reflected;
            }

            color_type impl(materials::physically_based const& mat) const
            {
                auto fresnel = fresnel_schlick(mat.ior, hit.shape_info.ray.dir, hit.shape_info.normal);
                auto reflect = mat.reflection * reflected;
                return mix(diffuse, reflect, fresnel);
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
        return apply_visitor(shader, scene.materials[hit.material_id]);
    }
}

