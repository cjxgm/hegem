#include "../lib/glm/vec3.hxx"
#include "../lib/glm/op/geom.hxx"
#include "../lib/glm/op/common.hxx"
#include "../global/counter.hxx"
#include "../raytracer/shade.hxx"
#include "../scene/material.hxx"
#include "../math/direction.hxx"
#include "../math/local-space.hxx"
#include "shade.hxx"
#include <cmath>

namespace hegem::pathtracer::shading_details
{
    inline namespace
    {
        namespace hits = raytracer::hits;
        namespace materials = scene::materials;
        using global::counter;
        using math::direction_type;

        float fresnel_schlick(float ior, direction_type const& viewing, direction_type const& normal)
        {
            auto base = (ior - 1) / (ior + 1);
            base *= base;
            auto exp = 1 - glm::abs(dot(*viewing, *normal));
            exp = exp*exp*exp*exp*exp;
            return base + (1-base)*exp;
        }

        auto importance_sample_GGX(
            direction_type normal,
            float roughness,
            math::uniform_sampler& canonical_sampler
        ) -> direction_type
        {
            auto s1 = canonical_sampler();
            auto s2 = canonical_sampler();
            auto polar = std::atan(roughness * std::sqrt(s1 / (1.0f - s1)));
            auto azimuth = 2.0f * math::pi * s2;
            auto cosp = std::cos(polar);
            auto sinp = std::sin(polar);
            auto cosa = std::cos(azimuth);
            auto sina = std::sin(azimuth);
            auto local = glm::vec3{
                sinp * cosa,
                sinp * sina,
                cosp,
            };
            auto local_to_world = math::local_space(normal);
            return local_to_world * local;
        }

        struct shader
        {
            hits::shape const& shape;
            math::uniform_sampler& canonical_sampler;

            template <class Material>
            auto operator () (Material const& mat) const -> shading_point
            {
                return impl(mat);
            }

        private:
            shading_point impl(materials::solid_color const& mat) const
            {
                return mat.color;
            }

            shading_point impl(materials::phong const& mat) const
            {
                return {};
            }

            shading_point impl(materials::physically_based const& mat) const
            {
                auto albedo = sample_albedo(mat.texture_pack, mat.albedo, shape.hit_point);
                auto roughness = sample_roughness(mat.texture_pack, mat.roughness, shape.hit_point);
                auto density = sample_density(mat.texture_pack, mat.density, shape.hit_point);

                // m is the microfacet normal
                auto m = importance_sample_GGX(shape.normal, roughness, canonical_sampler);

                auto fresnel = fresnel_schlick(mat.ior, shape.viewing.dir, m);
                fresnel = glm::mix(fresnel, 1.0f, mat.metalness);
                auto cosnv = dot(*shape.viewing.dir, *shape.normal);

                auto G1_GGX = [&] (direction_type dir) {
                    auto cosdm = dot(*dir, *m);
                    auto cosdn = dot(*dir, *shape.normal);
                    if (cosdm * cosdn <= 1e-5f) return 0.0f;

                    auto tan2dn = 1.0f / (cosdn * cosdn) - 1.0f;
                    auto r = glm::clamp(roughness, 0.0f, 1.0f);
                    return 2.0f / (1.0f + std::sqrt(1.0f + r * r * tan2dn));
                };

                auto G_GGX = [&] (direction_type o) {
                    return G1_GGX(-*shape.viewing.dir) * G1_GGX(o);
                };

                auto weight_of = [&] (direction_type o) {
                    // "x" means "don't care about signs"
                    auto xcosmi = dot(*shape.viewing.dir, *m);
                    auto xcosni = cosnv;
                    auto xcosmn = dot(*shape.normal, *m);
                    return std::abs(xcosmi / (xcosni * xcosmn)) * G_GGX(o);
                };

                auto into = (cosnv < 0.0f);
                if (into) {
                    if (canonical_sampler() <= fresnel) {     // reflection
                        counter.ray_refl++;

                        auto o = reflect(*shape.viewing.dir, *m);
                        auto shape_info_for_biasing = shape;
                        shape_info_for_biasing.normal = *m;

                        auto next_ray = biased_ray(ray_type{
                            shape.hit_point,
                            o,
                        }, shape_info_for_biasing);

                        return shading_point{
                            next_ray,
                            mat.reflection,
                            weight_of(o),
                            mat.emission,
                        };
                    } else {    // transmission (including diffuse)
                        if (canonical_sampler() > mat.opacity) {    // refraction
                            counter.ray_refr++;

                            auto eta = 1.0f / mat.ior;
                            auto o = refract(*shape.viewing.dir, *m, eta);
                            auto shape_info_for_biasing = shape;
                            shape_info_for_biasing.normal = -*m;

                            auto next_ray = biased_ray(ray_type{
                                shape.hit_point,
                                o,
                            }, shape_info_for_biasing);

                            return shading_point{
                                next_ray,
                                albedo,
                                weight_of(o),
                                mat.emission,
                            };
                        } else {    // diffuse
                            counter.ray_refl++;
                            auto o = math::sample_hemisphere(canonical_sampler, shape.normal);
                            auto next_ray = biased_ray(ray_type{
                                shape.hit_point,
                                o,
                            }, shape);
                            return shading_point{
                                next_ray,
                                albedo,
                                weight_of(o),
                                mat.emission,
                            };
                        }
                    }
                } else {
                    auto eta = mat.ior;
                    auto o = refract(*shape.viewing.dir, -*shape.normal, eta);
                    auto shape_info_for_biasing = shape;

                    if (std::isnan(o.x)) {  // total internal reflection
                        counter.ray_refl++;
                        o = reflect(*shape.viewing.dir, -*shape.normal);
                        shape_info_for_biasing.normal = -*shape.normal;
                    } else {
                        counter.ray_refr++;
                    }

                    auto next_ray = biased_ray(ray_type{
                        shape.hit_point,
                        o,
                    }, shape_info_for_biasing);

                    auto travel = shape.ray_extent;
                    auto color = exp(-travel*travel*density*(color_type{1.0f} - mat.albedo));

                    return shading_point{
                        next_ray,
                        color,
                        1.0f,
                        color_type{0.0f},
                    };
                }
            }
        };
    }

    auto shade(
        scene_type const& scene,
        object_hit_type const& hit,
        math::uniform_sampler& canonical_sampler
    ) -> shading_point
    {
        return hit.match(
            [&] (hits::missed const& hit) -> shading_point {
                return raytracer::shade_environment(scene, hit.viewing);
            },
            [&] (hits::object const& hit) {
                shader s{hit.shape_info, canonical_sampler};
                return apply_visitor(s, scene.materials[hit.material_id]);
            }
        );
    }
}

