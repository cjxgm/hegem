#include "../lib/glm/vec4.hh"
#include "raytracer.hh"
#include "intersect.hh"

namespace rt::raytracer
{
    path_type raytrace(scene::scene_type const& scene, ray_type const& ray, int remaining_bounce_count)
    {
        if (remaining_bounce_count < 0) return path_type{hits::missed{ray}, {}};

        return intersect(scene.root, ray).match(
            [&] (hits::missed m) { return path_type{m, {}}; },
            [&] (hits::object hit) {
                auto path = raytrace(scene, /* TODO: reflected lobe-sampled */ ray, remaining_bounce_count-1);
                path.hits.emplace_back(hit);
                return path;
            }
        );
    }

    path_buffer_type raytrace(scene::scene_type const& scene, int view_id, int max_bounce_count)
    {
        auto& view = scene.views[view_id];
        auto view_camera_transform = view.transformation();
        path_buffer_type buf{view.size()};

        buf.each([&] (auto& path, auto pos) {
            auto homo_origin       = view_camera_transform * glm::vec4{pos,  0, 1};
            auto homo_extent_point = view_camera_transform * glm::vec4{pos, -1, 1};
            auto origin       = homo_origin.xyz()       / homo_origin.w;
            auto extent_point = homo_extent_point.xyz() / homo_extent_point.w;
            ray_type ray{origin, extent_point - origin};
            path = raytrace(scene, ray, max_bounce_count);
        });

        return buf;
    }
}

