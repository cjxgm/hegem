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
#include <cmath>

namespace rt::raytracer::shading_details
{
    namespace
    {
        using direction_type = math::unit<glm::vec3>;
        namespace materials = scene::materials;

        float dot_clamp(glm::vec3 const& a, glm::vec3 const& b)
        {
            return glm::max(0.0f, dot(a, b));
        }

        float fresnel_schlick(float ior, direction_type const& viewing, direction_type const& normal)
        {
            auto base = (ior - 1) / (ior + 1);
            base *= base;
            auto exp = 1 - glm::abs(dot(*viewing, *normal));
            exp = exp*exp*exp*exp*exp;
            return base + (1-base)*exp;
        }

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
                direction_type v = -*viewing.dir;
                direction_type half = *to_lamp.dir + *v;
                auto nl = dot_clamp(*normal, *to_lamp.dir);
                auto nv = dot_clamp(*normal, *v);
                auto nh = dot_clamp(*normal, *half);
                auto vh = dot_clamp(*v, *half);
                auto nh2 = nh * nh;
                auto nh4 = nh2 * nh2;
                auto roughness = glm::max(glm::min(mat.roughness, 0.99999f), 0.00001f);
                auto slope = std::tan(roughness * float(M_PI) / 2.0f);
                auto slope2 = slope * slope;

                // Lambertian
                auto diffuse = mat.albedo / float(M_PI) * nl;

                // Beckmann distribution
                float distribution = glm::exp((nh2 - 1.0f) / (slope2 * nh2)) / (slope2 * nh4 * M_PI);

                // Cook Torrance geometry
                float geometry = glm::min(1.0f, 2.0f * nh / vh * glm::min(nv, nl));

                auto microfacet_fresnel = fresnel_schlick(mat.ior, v, half);
                auto spec = distribution * geometry * microfacet_fresnel / (4.0f * nv);
                auto specular = mat.reflection * spec;

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
            auto shadowed = is_intersected_within(scene.cache, shadow_ray, ulamp.distance_to_lamp);
            if (shadowed) continue;

            diffuse += apply_visitor(diffuse_term, mat) * ulamp.received_radiance;
        }
        return diffuse;
    }
}

