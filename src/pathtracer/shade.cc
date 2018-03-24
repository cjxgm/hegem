#include "../lib/glm/vec3.hh"
#include "../lib/glm/op/geom.hh"
#include "../lib/glm/op/common.hh"
#include "../raytracer/shade.hh"
#include "../scene/material.hh"
#include "../math/sampler.hh"
#include "../math/direction.hh"
#include "shade.hh"

namespace rt::pathtracer::shading_details
{
    namespace
    {
        namespace hits = raytracer::hits;
        namespace materials = scene::materials;
        using math::direction_type;

        thread_local math::uniform_sampler sample01{0.0f, 1.0f};

        float fresnel_schlick(float ior, direction_type const& viewing, direction_type const& normal)
        {
            auto base = (ior - 1) / (ior + 1);
            base *= base;
            auto exp = 1 - glm::abs(dot(*viewing, *normal));
            exp = exp*exp*exp*exp*exp;
            return base + (1-base)*exp;
        }

        struct shader
        {
            hits::shape const& shape;

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
                auto fresnel = fresnel_schlick(mat.ior, shape.viewing.dir, shape.normal);
                if (sample01() < fresnel) {     // transmission
                    auto next_ray = biased_ray(ray_type{
                        shape.hit_point,
                        math::sample_hemisphere(sample01, shape.normal),
                    }, shape);
                    return shading_point{
                        next_ray,
                        mat.albedo,
                        1.0f * fresnel, // TODO
                        color_type{},   // TODO
                    };
                } else {    // reflection
                    auto NV = dot(*shape.viewing.dir, *shape.normal);
                    auto into = (NV < 0.0f);
                    auto refl_normal = (into ? 1.0f : -1.0f) * *shape.normal;
                    auto dir = reflect(*shape.viewing.dir, refl_normal);
                    auto shape_info_for_biasing = shape;
                    shape_info_for_biasing.normal = refl_normal;

                    auto next_ray = biased_ray(ray_type{
                        shape.hit_point,
                        dir,
                    }, shape_info_for_biasing);

                    return shading_point{
                        next_ray,
                        mat.reflection,
                        -NV * (1.0f - fresnel),
                        color_type{},   // TODO
                    };
                }
            }
        };
    }

    auto shade(scene_type const& scene, object_hit_type const& hit) -> shading_point
    {
        return hit.match(
            [&] (hits::missed const& hit) -> shading_point {
                return raytracer::shade_environment(scene, hit.viewing);
            },
            [&] (hits::object const& hit) {
                shader s{hit.shape_info};
                return apply_visitor(s, scene.materials[hit.material_id]);
            }
        );
    }
}

