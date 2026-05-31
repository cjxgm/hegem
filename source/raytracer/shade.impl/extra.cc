#include "../../lib/glm/vec3.hh"
#include "../../lib/glm/vec4.hh"
#include "../../lib/glm/mat3.hh"
#include "../../lib/glm/op/geom.hh"
#include "../../math/direction.hh"
#include "../../math/constants.hh"
#include "../shade.hh"

namespace rt::raytracer::shading_details
{
    namespace
    {
        using math::direction_type;
        using math::inf;
        using lamp_container_type = scene::scene_type::lamp_container_type;
    }

    image_type shade_depth(hit_buffer_type const& buf, view_type const& view)
    {
        direction_type forward = view.camera.match([] (auto& cam) { return cam.forward; });

        image_type img{buf.size(), color_type{inf}};
        img.each([&] (auto& pixel, auto pos) {
            buf[pos].match(
                    [] (hits::missed) {},
                    [&] (hits::object const& hit) {
                        auto depth = hit.shape_info.ray_extent
                                * dot(*hit.shape_info.viewing.dir, *forward);
                        pixel = color_type{depth};
                    });
        });

        return img;
    }

    image_type shade_normal(hit_buffer_type const& buf, view_type const& view)
    {
        auto c2w = scene::camera_space_to_world_space(view.camera);
        auto w2c = inverse(c2w);

        image_type img{buf.size(), color_type{0.5, 0.5, 1}};
        img.each([&] (auto& pixel, auto pos) {
            buf[pos].match(
                    [] (hits::missed) {},
                    [&] (hits::object const& hit) {
                        direction_type view_normal = glm::vec3{w2c * glm::vec4{*hit.shape_info.normal, 0.0f}};
                        pixel = *view_normal * 0.5f + 0.5f;
                    });
        });

        return img;
    }
}

