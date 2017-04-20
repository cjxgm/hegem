#include "../../lib/glm/vec3.hh"
#include "../../lib/glm/mat3.hh"
#include "../../lib/glm/op/geom.hh"
#include "../../math/unit.hh"
#include "../shade.hh"
#include <limits>

namespace rt::raytracer::shading_details
{
    namespace
    {
        using direction_type = math::unit<glm::vec3>;
        using lamp_container_type = scene::scene_type::lamp_container_type;
        static constexpr auto inf = std::numeric_limits<float>::infinity();
    }

    image_type shade_depth(hit_buffer_type const& buf, scene_type const& scene, int view_id)
    {
        auto& view = scene.views[view_id];
        direction_type forward = view.camera.match([] (auto& cam) { return cam.forward; });

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
        auto c2w_rot = scene::camera_space_to_world_space_rotation_only(view.camera);
        auto w2c_rot = inverse(c2w_rot);

        image_type img{buf.size(), color_type{0.5, 0.5, 1}};
        img.each([&] (auto& pixel, auto pos) {
            buf[pos].match(
                    [] (hits::missed) {},
                    [&] (hits::object const& hit) {
                        direction_type view_normal = w2c_rot * *hit.shape_info.normal;
                        pixel = *view_normal * 0.5f + 0.5f;
                    });
        });

        return img;
    }
}

