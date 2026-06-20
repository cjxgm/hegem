#include "../../lib/glm/vec3.hxx"
#include "../../lib/glm/op/common.hxx"
#include "../../lib/glm/op/geom.hxx"
#include "../../scene/material.hxx"
#include "../../math/direction.hxx"
#include "../shade.hxx"
#include <stdexcept>

namespace hegem::raytracer::shading_details
{
    namespace
    {
        namespace materials = scene::materials;
        using math::direction_type;

        float fresnel_schlick(float ior, direction_type viewing, direction_type normal)
        {
            auto base = (ior - 1) / (ior + 1);
            base *= base;
            auto exp = 1 - glm::abs(dot(*viewing, *normal));
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
            color_type impl(materials::solid_color const& mat) const
            {
                return diffuse;
            }

            color_type impl(materials::phong const& mat) const
            {
                return diffuse + mat.reflection * reflected + mat.refraction * refracted;
            }

            color_type impl(materials::physically_based const& mat) const
            {
                auto fresnel = fresnel_schlick(mat.ior, hit.shape_info.viewing.dir, hit.shape_info.normal);
                auto refract = mat.albedo * refracted;
                auto reflect = mat.reflection * reflected;
                auto tx = mix(refract, diffuse, mat.opacity);
                return mix(tx, reflect, fresnel);
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

