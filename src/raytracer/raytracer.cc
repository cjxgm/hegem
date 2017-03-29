#include "raytracer.hh"
#include "intersect.hh"

#define WANT_VEC4
#include "../lib/glm.hh"
#undef WANT_VEC4

namespace rt::raytracer
{
    path_type raytrace(scene::scene_type const& scene, ray_type const& ray, int remaining_bounce_count)
    {
        return intersect(scene.root, ray).match(
            [] (hits::missed) { return path_type{}; },
            [&] (hits::object hit) {
                auto path = remaining_bounce_count > 0
                        ? raytrace(scene, /* TODO: reflected lobe-sampled */ ray, remaining_bounce_count-1)
                        : path_type{};
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

