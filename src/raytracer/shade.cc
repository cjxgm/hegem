#include "../lib/glm/vec3.hh"
#include "../lib/glm/mat3.hh"
#include "../math/unit.hh"
#include "../image/color.hh"
#include "shade.hh"
#include <limits>

namespace rt::raytracer::shading_details
{
    namespace
    {
        using direction_type = math::unit<glm::vec3>;
        using image::color::linear_rgb;
        using image::color::gamma_to_internal;
        static constexpr auto inf = std::numeric_limits<float>::infinity();
    }

    image_type shade(path_buffer_type const& buf, scene_type const& scene)
    {
        image_type img{buf.size(), linear_rgb{0.8, 0.4, 0.1}};
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

