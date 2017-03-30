#include "../lib/glm/vec3.hh"
#include "../lib/glm/mat3.hh"
#include "../math/unit.hh"
#include "../image/color.hh"
#include "../scene/material.hh"
#include "../scene/scene.hh"
#include "shade.hh"
#include "hit.hh"
#include <limits>
#include <stdexcept>

namespace rt::raytracer::shading_details
{
    namespace
    {
        using direction_type = math::unit<glm::vec3>;
        using lamp_container_type = scene::scene_type::lamp_container_type;
        using image::color::linear_rgb;
        using image::color::gamma_to_internal;
        static constexpr auto inf = std::numeric_limits<float>::infinity();

        inline namespace shade_path_details
        {
            inline namespace shade_missed
            {
                linear_rgb shade_path(
                        hits::missed const& /*miss*/,
                        scene::materials::solid_color const& mat,
                        lamp_container_type const& /*lamps*/)
                {
                    return mat.color;
                }

                linear_rgb shade_path(
                        hits::missed const& miss,
                        scene::materials::phong const& mat,
                        lamp_container_type const& lamps)
                {
                    throw std::runtime_error{"Environment cannot have a phong material."};
                }

                linear_rgb shade_path(
                        hits::missed const& miss,
                        scene::materials::physically_based const& mat,
                        lamp_container_type const& lamps)
                {
                    throw std::runtime_error{"Environment cannot have a PBR material."};
                }
            }

            inline namespace shade_hit_object
            {
                linear_rgb shade_path(
                        hits::object const& /*hit*/,
                        scene::materials::solid_color const& mat,
                        lamp_container_type const& /*lamps*/,
                        linear_rgb const& /*shaded*/)
                {
                    return mat.color;
                }

                linear_rgb shade_path(
                        hits::object const& hit,
                        scene::materials::phong const& mat,
                        lamp_container_type const& lamps,
                        linear_rgb const& shaded)
                {
                    if (lamps.empty()) {    // This is only for Assignment 1 compatibility
                        return mat.diffuse;
                    } else {
                        throw std::runtime_error{"TODO"};
                    }
                }

                linear_rgb shade_path(
                        hits::object const& hit,
                        scene::materials::physically_based const& mat,
                        lamp_container_type const& lamps,
                        linear_rgb const& shaded)
                {
                    throw std::runtime_error{"TODO"};
                }
            }
        }
    }

    image_type shade(path_buffer_type const& buf, scene_type const& scene)
    {
        image_type img{buf.size()};
        auto& env_mat = scene.materials[scene.environment];
        auto& lamps = scene.lamps;

        img.each([&] (auto& pixel, auto pos) {
            auto& hits = buf[pos].hits;
            auto& miss = buf[pos].miss;

            pixel = env_mat.match(
                    [&] (auto& mat) {
                        return shade_path(miss, mat, lamps);
                    });

            for (auto& hit: hits) {
                auto& mat = scene.materials[hit.material_id];
                pixel = mat.match(
                        [&] (auto& mat) {
                            return shade_path(hit, mat, lamps, pixel);
                        });
            }
        });
        return img;
    }

    image_type shade_depth(path_buffer_type const& buf, scene_type const& scene, int view_id)
    {
        auto& view = scene.views[view_id];
        auto view_camera_transform = view.transformation();

        // Don't ask me why. It's just this.
        // See src/scene/camera.cc to figure out why.
        direction_type forward = -view_camera_transform[2].xyz();

        image_type img{buf.size(), linear_rgb{inf}};
        img.each([&] (auto& pixel, auto pos) {
            auto& hits = buf[pos].hits;
            if (hits.empty()) return;
            auto& hit = hits.back();
            auto depth = hit.geom.ray_extent * dot(*hit.geom.ray.dir, *forward);
            pixel = linear_rgb{depth};
        });

        return img;
    }

    image_type shade_normal(path_buffer_type const& buf, scene_type const& scene, int view_id)
    {
        auto& view = scene.views[view_id];
        auto inv_camera_rot = inverse(view.camera_rotation());

        image_type img{buf.size(), linear_rgb{0.5, 0.5, 1}};
        img.each([&] (auto& pixel, auto pos) {
            auto& hits = buf[pos].hits;
            if (hits.empty()) return;
            auto& hit = hits.back();
            direction_type view_normal = inv_camera_rot * *hit.geom.normal;
            pixel = *view_normal * 0.5f + 0.5f;
        });

        return img;
    }
}

