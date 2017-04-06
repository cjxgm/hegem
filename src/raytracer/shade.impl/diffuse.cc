#include "../../lib/glm/vec3.hh"
#include "../../lib/glm/op/geom.hh"
#include "../../lib/glm/op/common.hh"
#include "../../math/unit.hh"
#include "../../scene/material.hh"
#include "../../scene/lamp.hh"
#include "../shade.hh"
#include "../intersect.hh"
#include <limits>
#include <stdexcept>

namespace rt::raytracer::shading_details
{
    namespace
    {
        using direction_type = math::unit<glm::vec3>;
        namespace materials = scene::materials;
        namespace lamps = scene::lamps;
        static constexpr auto inf = std::numeric_limits<float>::infinity();

        struct lamp_info_extractor
        {
            hits::object const& hit;

            color_type received_radiance{};
            ray_type towards_lamp{};
            float distance_to_lamp{};

            template <class Lamp>
            void operator () (Lamp const& lamp)
            {
                impl(lamp);
            }

        private:
            void impl(lamps::sun const& lamp)
            {
                received_radiance = lamp.color;
                towards_lamp = ray_type {
                    hit.shape_info.hit_point,
                    -*lamp.dir,
                };
                distance_to_lamp = inf;
            }

            void impl(lamps::omni const& lamp)
            {
                auto obj_to_lamp = lamp.center - hit.shape_info.hit_point;
                auto dist = length(obj_to_lamp) + 1;  // distance starts at 1

                received_radiance = lamp.color / (dist * dist);
                towards_lamp = ray_type {
                    hit.shape_info.hit_point,
                    obj_to_lamp,
                };
                distance_to_lamp = dist;
            }
        };

        // diffuse AND probably specular terms
        struct diffuse_term_extractor
        {
            ray_type const& to_lamp;
            ray_type const& ray;
            direction_type const& normal;

            template <class Material>
            auto operator () (Material const& mat) const
            {
                return impl(mat);
            }

        private:
            color_type impl(materials::solid_color const& mat) const
            {
                throw std::logic_error{"unreachable"};
            }

            // Blinn-phong
            color_type impl(materials::phong const& mat) const
            {
                auto nl = glm::max(dot(*normal, *to_lamp.dir), 0.0f);
                auto diffuse = mat.diffuse * nl;

                direction_type half = *to_lamp.dir - *ray.dir;
                auto nh = glm::max(dot(*normal, *half), 0.0f);     // FIXME: is this `max` necessary?
                auto specular = glm::pow(nh, mat.specular_exp) * mat.specular * nl;

                return diffuse + specular;
            }

            color_type impl(materials::physically_based const& mat) const
            {
                throw std::logic_error{"TODO"};
            }
        };
    }

    color_type shade_diffuse(scene_type const& scene, hits::object const& hit)
    {
        lamp_info_extractor lamp_info{hit};

        color_type diffuse;
        auto& mat = scene.materials[hit.material_id];
        for (auto& lamp: scene.lamps) {
            apply_visitor(lamp_info, lamp);
            diffuse_term_extractor diffuse_term{
                lamp_info.towards_lamp,
                hit.shape_info.ray,
                hit.shape_info.normal,
            };

            auto shadowed = intersect(scene.root, lamp_info.towards_lamp).match(
                    [] (hits::missed) { return false; },
                    [&] (hits::object hit) {
                        return (hit.shape_info.ray_extent < lamp_info.distance_to_lamp);
                    });
            if (shadowed) continue;

            diffuse += apply_visitor(diffuse_term, mat) * lamp_info.received_radiance;
        }
        return diffuse;
    }
}

