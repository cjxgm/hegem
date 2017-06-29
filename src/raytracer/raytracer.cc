#include "../lib/glm/op/geom.hh"
#include "../lib/glm/mat3.hh"
#include "../lib/glm/vec2.hh"
#include "../lib/glm/vec3.hh"
#include "raytracer.hh"
#include "intersect.hh"
#include "shade.hh"
#include <limits>

namespace rt::raytracer::raytracer_details
{
    namespace
    {
        static constexpr auto inf = std::numeric_limits<float>::infinity();
        static constexpr auto color_primary_ray = color_type{10.0f, 5.0f, 2.0f};
        static constexpr auto width_primary_ray = 10.0f;
        static constexpr auto color_reflective_ray = color_type{10.0f, 2.0f, 5.0f};
        static constexpr auto width_reflective_ray = 8.0f;
        static constexpr auto color_refractive_ray = color_type{5.0f, 2.0f, 10.0f};
        static constexpr auto width_refractive_ray = 8.0f;
        static constexpr auto color_normal_ray = color_type{2.0f, 5.0f, 10.0f};
        static constexpr auto width_normal_ray = 10.0f;
        static constexpr auto length_normal_ray = 0.3f;

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
                        ray_type refl = biased_ray({
                            hit.shape_info.hit_point,
                            reflect(*ray.dir, *hit.shape_info.normal),
                        }, hit.shape_info);
                        reflected = raytrace(scene, refl, remaining_bounce_count).radiance;
                    }
                    if (refractive(mat)) {
                        refracted = raytrace(scene, /* refract */ biased_ray(ray, hit.shape_info), remaining_bounce_count).radiance;
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

        void raytrace(
                scene_type const& scene,
                ray_type const& ray,
                int remaining_bounce_count,
                ray_visualizations & rvs,
                color_type ray_color,
                float ray_width)
        {
            if (remaining_bounce_count < 0) {
                rvs.emplace_back(ray, +inf, ray_color, ray_width);
                return;
            }
            remaining_bounce_count--;

            return intersect(scene.root, ray).match(
                [&] (hits::missed m) {
                    rvs.emplace_back(ray, +inf, ray_color, ray_width);
                },
                [&] (hits::object hit) {
                    rvs.emplace_back(ray, hit.shape_info.ray_extent, ray_color, ray_width);

                    ray_type normal_ray{
                        .origin = hit.shape_info.hit_point,
                        .dir = hit.shape_info.normal,
                    };
                    rvs.emplace_back(normal_ray, length_normal_ray, color_normal_ray, width_normal_ray);

                    auto& mat = scene.materials[hit.material_id];
                    if (reflective(mat)) {
                        ray_type refl = biased_ray({
                            hit.shape_info.hit_point,
                            reflect(*ray.dir, *hit.shape_info.normal),
                        }, hit.shape_info);
                        raytrace(
                            scene,
                            refl,
                            remaining_bounce_count,
                            rvs,
                            color_reflective_ray,
                            width_reflective_ray);
                    }
                    if (refractive(mat)) {
                        raytrace(
                            scene,
                            /* refract */ biased_ray(ray, hit.shape_info),
                            remaining_bounce_count,
                            rvs,
                            color_refractive_ray,
                            width_refractive_ray);
                    }
                }
            );
        }
    }

    raytracing_result_type raytrace(
            scene_type const& scene,
            view_type const& view,
            util::tile const& tile)
    {
        auto& cam = view.camera;
        auto s2cp = view.screen_space_to_camera_plane_space();

        image_type img{{tile.w, tile.h}};
        hit_buffer_type buf{{tile.w, tile.h}};

        buf.each([&] (auto& hit, auto pos) {
            auto p = s2cp * glm::vec3{pos + glm::ivec2{tile.x, tile.y}, 1};
            auto ray = camera_ray_from_camera_plane(p.xy(), cam);
            auto shaded_hit = raytrace(scene, ray, view.bounces);
            hit = shaded_hit.hit;
            img[pos] = shaded_hit.radiance;
        });

        return { img, buf };
    }

    ray_visualizations raytrace(scene_type const& scene, view_type const& view, glm::vec2 screen_pos)
    {
        ray_visualizations rvs{};

        auto& cam = view.camera;
        auto s2cp = view.screen_space_to_camera_plane_space();
        auto p = s2cp * glm::vec3{screen_pos, 1.0f};
        auto ray = camera_ray_from_camera_plane(p.xy(), cam);
        raytrace(scene, ray, view.bounces, rvs, color_primary_ray, width_primary_ray);

        return rvs;
    }
}

