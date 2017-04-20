#include "../lib/glm/op/geom.hh"
#include "../lib/glm/mat3.hh"
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
                    ray_type refl{
                        hit.shape_info.hit_point,
                        reflect(*ray.dir, *hit.shape_info.normal),
                    };
                    reflected = raytrace(scene, refl, remaining_bounce_count).radiance;
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
            view_type const& view,
            util::tile const& tile)
    {
        auto& cam = view.camera;
        auto s2cp = view.screen_space_to_camera_plane_space();

        image_type img{{tile.w, tile.h}};
        hit_buffer_type buf{{tile.w, tile.h}};

        buf.each([&] (auto& hit, auto pos) {
            auto p = s2cp * glm::vec3{pos + glm::ivec2{tile.x, tile.y}, 1};
            auto ray = camera_ray_from_camera_plane(p, cam);
            auto shaded_hit = raytrace(scene, ray, view.bounces);
            hit = shaded_hit.hit;
            img[pos] = shaded_hit.radiance;
        });

        return { img, buf };
    }
}

