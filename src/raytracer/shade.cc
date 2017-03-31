#include "../lib/glm/vec3.hh"
#include "../lib/glm/mat3.hh"
#include "../lib/glm/op/geom.hh"
#include "../math/unit.hh"
#include "../scene/material.hh"
#include "../scene/lamp.hh"
#include "shade.hh"
#include "intersect.hh"
#include <limits>
#include <stdexcept>

namespace rt::raytracer::shading_details
{
    namespace
    {
        using direction_type = math::unit<glm::vec3>;
        using lamp_container_type = scene::scene_type::lamp_container_type;
        namespace materials = scene::materials;
        namespace lamps = scene::lamps;
        static constexpr auto inf = std::numeric_limits<float>::infinity();
    }

    image_type shade_depth(hit_buffer_type const& buf, scene_type const& scene, int view_id)
    {
        auto& view = scene.views[view_id];
        auto view_camera_transform = view.transformation();

        // Don't ask me why. It's just this.
        // See src/scene/camera.cc to figure out why.
        direction_type forward = -view_camera_transform[2].xyz();

        image_type img{buf.size(), color_type{inf}};
        img.each([&] (auto& pixel, auto pos) {
            buf[pos].match(
                    [] (hits::missed) {},
                    [&] (hits::object const& hit) {
                        auto depth = hit.shape_info.ray_extent
                                * dot(*hit.shape_info.ray.dir, *forward);
                        pixel = color_type{depth};
                    });
        });

        return img;
    }

    image_type shade_normal(hit_buffer_type const& buf, scene_type const& scene, int view_id)
    {
        auto& view = scene.views[view_id];
        auto inv_camera_rot = inverse(view.camera_rotation());

        image_type img{buf.size(), color_type{0.5, 0.5, 1}};
        img.each([&] (auto& pixel, auto pos) {
            buf[pos].match(
                    [] (hits::missed) {},
                    [&] (hits::object const& hit) {
                        direction_type view_normal = inv_camera_rot
                                * *hit.shape_info.normal;
                        pixel = *view_normal * 0.5f + 0.5f;
                    });
        });

        return img;
    }

    namespace
    {
        struct environment_shader
        {
            scene_type const& scene;
            ray_type const& ray;

            template <class Material>
            auto operator () (Material const& mat) const
            {
                return impl(mat);
            }

        private:
            color_type impl(materials::solid_color const& mat) const
            {
                return mat.color;
            }

            color_type impl(materials::phong const& mat) const
            {
                throw std::runtime_error{"Phong materials cannot be used by environment."};
            }

            color_type impl(materials::physically_based const& mat) const
            {
                throw std::runtime_error{"PBR materials cannot be used by environment."};
            }
        };
    }

    color_type shade_environment(scene_type const& scene, ray_type const& ray)
    {
        environment_shader shader{scene, ray};
        return scene.materials[scene.environment].match(shader);
    }

    namespace
    {
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

    namespace
    {
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

