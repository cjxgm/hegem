#include "../lib/std/optional.hxx"
#include "../lib/glm/vec2.hxx"
#include "../lib/glm/vec3.hxx"
#include "../math/sampler.hxx"
#include "../raytracer/intersect.hxx"
#include "../raytracer/ray.hxx"
#include "shade.hxx"
#include "pathtracer.hxx"
#include <utility>      // for std::move

namespace rt::pathtracer::pathtracer_details
{
    namespace
    {
        using global::counter;
        using color_type = image_type::color_type;
        using raytracer::ray_type;

        struct pathtracer_impl
        {
            scene_type const& scene;
            math::uniform_sampler canonical_sampler{0.0f, 1.0f};

            auto trace_path(lib::optional<ray_type> viewing, int remaining_bounces) -> color_type
            {
                color_type radiance{0.0f};
                color_type filter{1.0f};

                while (remaining_bounces-- > 0 && viewing) {
                    counter.ray++;

                    auto hit = raytracer::intersect(scene.cache, *viewing);
                    auto shading_point = shade(scene, hit, canonical_sampler);

                    viewing = std::move(shading_point.next_ray);
                    radiance += shading_point.emission * filter;
                    filter *= shading_point.ray_color;

                    if (filter.x == 0.0f && filter.y == 0.0f && filter.z == 0.0f) break;
                }

                return radiance;
            }
        };
    }

    auto pathtrace(scene_type const& scene, view_type const& view, util::tile const& tile, update_fn update) -> image_type
    {
        math::normal_sampler pixel_jitter{0, 0.2};
        pathtracer_impl impl{scene};
        auto& cam = view.camera;
        auto s2cp = view.screen_space_to_camera_plane_space();

        image_type img{{tile.w, tile.h}};
        image_type img_per_sample{{tile.w, tile.h}};

        const int max_samples = view.samples > 0 ? view.samples : 1;
        for (int sample=0; sample<max_samples; sample++) {
            img.each([&] (auto& color, auto pos) {
                auto screen_pos = glm::vec2{pos + glm::ivec2{tile.x, tile.y}};
                screen_pos.x += pixel_jitter();
                screen_pos.y += pixel_jitter();
                auto p = s2cp * glm::vec3{screen_pos, 1};
                auto ray = raytracer::camera_ray_from_camera_plane(glm::vec2{p}, cam);
                auto& sample_color = img_per_sample[pos];
                sample_color = impl.trace_path(ray, view.bounces);

                color *= float(sample);
                color += sample_color;
                color /= float(sample + 1);
            });

            if (update) update(img_per_sample);
        }

        return img;
    }
}

