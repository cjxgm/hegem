#include "../../lib/glm/vec3.hxx"
#include "../../lib/glm/op/geom.hxx"
#include "../../lib/glm/op/common.hxx"
#include "../../tool/unreachable.macro.hxx"
#include "../../math/direction.hxx"
#include "../../scene/material.hxx"
#include "../../scene/lamp.hxx"
#include "../../global/counter.hxx"
#include "../shade.hxx"
#include "../intersect.hxx"
#include "unified-lamp.hxx"
#include <limits>

namespace hegem::raytracer::shading_details
{
    namespace
    {
        using global::counter;
        using math::direction_type;
        namespace materials = scene::materials;
        static constexpr auto clamp_eps = 1e-7f;

        float dot_clamp(glm::vec3 const& a, glm::vec3 const& b)
        {
            return glm::clamp(dot(a, b), clamp_eps, 1.0f - clamp_eps);
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
            glm::vec3 const& hit_point;

            template <class Material>
            auto operator () (Material const& mat) const
            {
                return impl(mat);
            }

        private:
            color_type impl(materials::solid_color const& /*mat*/) const
            {
                RT_UNREACHABLE();
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
                auto slope = sample_roughness(mat.texture_pack, mat.roughness, hit_point);
                if (slope < 1e-5f) slope = 1e-5f;
                auto slope2 = slope * slope;

                // Lambertian
                auto albedo = sample_albedo(mat.texture_pack, mat.albedo, hit_point);
                auto diffuse = albedo / math::pi * nl;

                // Beckmann distribution
                float distribution = glm::exp((nh2 - 1.0f) / (slope2 * nh2)) / (slope2 * nh4 * math::pi);

                // Cook Torrance geometry
                float geometry = glm::min(1.0f, 2.0f * nh / vh * glm::min(nv, nl));

                auto microfacet_fresnel = fresnel_schlick(mat.ior, v, half);
                auto spec = distribution * geometry * microfacet_fresnel / (4.0f * nv);
                auto specular = mat.reflection * spec;

                return diffuse + specular;
            }
        };
    }

    color_type shade_diffuse(scene_type const& scene, hits::object const& hit, math::normal_sampler & samp)
    {
        color_type diffuse;
        auto& mat = scene.materials[hit.material_id];
        for (auto& lamp: scene.lamps) {
            auto ulamp = unify_lamp(lamp, hit, samp);
            diffuse_term_extractor diffuse_term{
                ulamp.towards_lamp,
                hit.shape_info.viewing,
                hit.shape_info.normal,
                hit.shape_info.hit_point,
            };

            counter.ray++;
            counter.ray_shadow++;
            auto shadow_ray = biased_ray(ulamp.towards_lamp, hit.shape_info);
            auto shadowed = is_intersected_within(scene.cache, shadow_ray, ulamp.distance_to_lamp);
            if (shadowed) continue;

            diffuse += apply_visitor(diffuse_term, mat) * ulamp.received_radiance;
        }
        return diffuse;
    }
}

