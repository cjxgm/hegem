#include "../lib/glm/vec4.hh"
#include "../scene/material.hh"
#include "../image/color.hh"
#include "raytracer.hh"
#include "intersect.hh"
#include "shade.hh"

namespace rt::raytracer::raytracer_details
{
    namespace
    {
        using color_type = image::color::linear_rgb;
    }

    shaded_object_hit_type raytrace(
            scene_type const& scene,
            ray_type const& ray,
            int remaining_bounce_count)
    {
        if (remaining_bounce_count < 0) {
            auto radiance = shade_environment(scene, ray);
            return shaded_object_hit_type {
                hits::missed{ray},
                radiance,
            };
        }
        remaining_bounce_count--;

        return intersect(scene.root, ray).match(
            [&] (hits::missed m) {
                auto radiance = shade_environment(scene, m.ray);
                return shaded_object_hit_type {
                    m,
                    radiance,
                };
            },
            [&] (hits::object hit) {
                color_type reflected;
                color_type refracted;

                auto& mat = scene.materials[hit.material_id];
                if (reflective(mat)) {
                    reflected = raytrace(scene, /* reflect */ ray, remaining_bounce_count).radiance;
                }
                if (refractive(mat)) {
                    refracted = raytrace(scene, /* refract */ ray, remaining_bounce_count).radiance;
                }

                auto diffuse = shade_diffuse(scene, hit);
                auto radiance = shade_illumination(scene, hit, diffuse, reflected, refracted);

                return shaded_object_hit_type {
                    hit,
                    radiance,
                };
            }
        );
    }

    raytracing_result_type raytrace(
            scene_type const& scene,
            int view_id,
            int max_bounce_count)
    {
        auto& view = scene.views[view_id];
        auto view_camera_transform = view.transformation();
        image_type img{view.size()};
        hit_buffer_type buf{view.size()};

        buf.each([&] (auto& hit, auto pos) {
            auto homo_origin       = view_camera_transform * glm::vec4{pos,  0, 1};
            auto homo_extent_point = view_camera_transform * glm::vec4{pos, -1, 1};
            auto origin       = homo_origin.xyz()       / homo_origin.w;
            auto extent_point = homo_extent_point.xyz() / homo_extent_point.w;
            ray_type ray{origin, extent_point - origin};

            auto shaded_hit = raytrace(scene, ray, max_bounce_count);
            hit = shaded_hit.hit;
            img[pos] = shaded_hit.radiance;
        });

        return { img, buf };
    }
}

