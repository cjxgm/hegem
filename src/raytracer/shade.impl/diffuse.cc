#include "../../lib/glm/vec3.hh"
#include "../../lib/glm/op/geom.hh"
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
            float impl(materials::solid_color const& mat) const
            {
                throw std::logic_error{"unreachable"};
            }

            float impl(materials::phong const& mat) const
            {
                // TODO
                return 1;
            }

            float impl(materials::physically_based const& mat) const
            {
                throw std::logic_error{"TODO"};
            }
        };
    }

    color_type shade_diffuse(scene_type const& scene, hits::object const& hit)
    {
        lamp_info_extractor lamp_info{hit};
        diffuse_term_extractor diffuse_term{lamp_info.towards_lamp, hit.shape_info.ray, hit.shape_info.normal};

        color_type diffuse;
        auto& mat = scene.materials[hit.material_id];
        for (auto& lamp: scene.lamps) {
            lamp.match(lamp_info);

            auto shadowed = intersect(scene.root, lamp_info.towards_lamp).match(
                    [] (hits::missed) { return false; },
                    [&] (hits::object hit) {
                        return (hit.shape_info.ray_extent < lamp_info.distance_to_lamp);
                    });
            if (shadowed) continue;

            diffuse += mat.match(diffuse_term) * lamp_info.received_radiance;
        }
        return diffuse;
    }
}

