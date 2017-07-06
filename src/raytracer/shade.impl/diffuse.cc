#include "../../lib/glm/vec3.hh"
#include "../../lib/glm/op/geom.hh"
#include "../../lib/glm/op/common.hh"
#include "../../math/unit.hh"
#include "../../scene/material.hh"
#include "../../scene/lamp.hh"
#include "../shade.hh"
#include "../intersect.hh"
#include "unified-lamp.hh"
#include <limits>
#include <stdexcept>

namespace rt::raytracer::shading_details
{
    namespace
    {
        using direction_type = math::unit<glm::vec3>;
        namespace materials = scene::materials;

        // diffuse AND probably specular terms
        struct diffuse_term_extractor
        {
            ray_type const& to_lamp;
            ray_type const& viewing;
            direction_type const& normal;

            template <class Material>
            auto operator () (Material const& mat) const
            {
                return impl(mat);
            }

        private:
            color_type impl(materials::solid_color const& /*mat*/) const
            {
                throw std::logic_error{"unreachable"};
            }

            // Blinn-phong
            color_type impl(materials::phong const& mat) const
            {
                auto nl = glm::max(dot(*normal, *to_lamp.dir), 0.0f);
                auto diffuse = mat.diffuse * nl;

                direction_type half = *to_lamp.dir - *viewing.dir;
                auto nh = glm::max(dot(*normal, *half), 0.0f);     // FIXME: is this `max` necessary?
                auto specular = glm::pow(nh, mat.specular_exp) * mat.specular * nl;

                return diffuse + specular;
            }

            color_type impl(materials::physically_based const& mat) const
            {
                // FIXME: Temp. use Blinn-phong
                auto nl = glm::max(dot(*normal, *to_lamp.dir), 0.0f);
                auto diffuse = mat.albedo * nl;

                direction_type half = *to_lamp.dir - *viewing.dir;
                auto nh = glm::max(dot(*normal, *half), 0.0f);     // FIXME: is this `max` necessary?
                auto exp = (1.0f / (mat.roughness + 1.0f) - 0.5f) * 100.0f;
                auto specular = glm::pow(nh, exp) * mat.reflection * nl;

                return diffuse + specular;
            }
        };
    }

    color_type shade_diffuse(scene_type const& scene, hits::object const& hit)
    {
        color_type diffuse;
        auto& mat = scene.materials[hit.material_id];
        for (auto& lamp: scene.lamps) {
            auto ulamp = unify_lamp(lamp, hit);
            diffuse_term_extractor diffuse_term{
                ulamp.towards_lamp,
                hit.shape_info.viewing,
                hit.shape_info.normal,
            };

            auto shadow_ray = biased_ray(ulamp.towards_lamp, hit.shape_info);
            auto shadowed = is_intersected_within(scene.root, shadow_ray, ulamp.distance_to_lamp);
            if (shadowed) continue;

            diffuse += apply_visitor(diffuse_term, mat) * ulamp.received_radiance;
        }
        return diffuse;
    }
}

