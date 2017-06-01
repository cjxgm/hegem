#include "../lib/glm/vec4.hh"
#include "../lib/glm/mat3.hh"
#include "../lib/glm/mat4.hh"
#include "../lib/glm/op/geom.hh"
#include "../lib/glm/op/mat.hh"
#include "camera.hh"

namespace rt::scene::cameras
{
    glm::mat3 camera_space_to_world_space_rotation_only(camera_type const& cam)
    {
        return cam.match([] (auto& cam) {
            direction_type right = cross(*cam.forward, *cam.up);
            direction_type proper_up = cross(*right, *cam.forward);
            return glm::mat3{
                *right,
                *proper_up,
                -*cam.forward,
            };
        });
    }

    glm::mat4 camera_space_to_world_space(camera_type const& cam)
    {
        return cam.match([&] (auto& cam_alt) {
            auto c2w_rot = camera_space_to_world_space_rotation_only(cam);
            return glm::mat4{
                glm::vec4{c2w_rot[0]    , 0},
                glm::vec4{c2w_rot[1]    , 0},
                glm::vec4{c2w_rot[2]    , 0},
                glm::vec4{cam_alt.center, 1},
            };
        });
    }

    glm::mat4 world_space_to_clip_space(camera_type const& cam, float aspect_ratio)
        // TODO: allow user to adjust far plane?
    {
        // aspect corrected normalized width and height
        auto nw = glm::max(aspect_ratio, 1.0f);
        auto nh = glm::min(aspect_ratio, 1.0f);

        auto proj = cam.match(
            [=] (pin_hole const& cam) {
                float w = 2.0f * nw;
                float h = 2.0f * nh;
                return glm::perspectiveFov(cam.fov, w, h, 0.0f, 1000.0f);
            },
            [=] (orthographic const& cam) {
                float xmax = cam.size / 2.0f * nw;
                float ymax = cam.size / 2.0f * nh;
                return glm::ortho(-xmax, xmax, -ymax, ymax, 0.0f, 1000.0f);
            });

        auto c2w = camera_space_to_world_space(cam);
        auto w2c = inverse(c2w);

        return proj * w2c;
    }
}

