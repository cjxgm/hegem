#include "../lib/glm/vec4.hh"
#include "../lib/glm/mat3.hh"
#include "../lib/glm/mat4.hh"
#include "../lib/glm/op/geom.hh"
#include "../lib/glm/op/mat.hh"
#include "camera.hh"
#include <cmath>

namespace rt::scene::cameras
{
    namespace
    {
        // Compensate a graphical environment that does not have
        // abstractions over camera lenses.
        //
        // The raytracer assumes that the "lens" has a size.
        // The rasterizer thinks that the "lens" is just an infinitesimal pin hole.
        glm::mat4 sized_lens_to_pin_hole(float aspect_corrected_h, float yfov)
        {
            auto offset = aspect_corrected_h / std::tan(yfov / 2.0f);
            return {
                glm::vec4{1, 0, 0, 0},
                glm::vec4{0, 1, 0, 0},
                glm::vec4{0, 0, 1, 0},
                glm::vec4{0, 0, -offset, 1},
            };
        }
    }

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
        auto nw = aspect_ratio > 1.0f ?        aspect_ratio : 1.0f;
        auto nh = aspect_ratio < 1.0f ? 1.0f / aspect_ratio : 1.0f;

        auto proj = cam.match(
            [=] (pin_hole const& cam) {
                float w = 2.0f * nw;
                float h = 2.0f * nh;
                auto s2p = sized_lens_to_pin_hole(nh, cam.fov);
                return glm::perspectiveFov(cam.fov, w, h, 1e-5f, 1e2f) * s2p;
            },
            [=] (orthographic const& cam) {
                float xmax = cam.size / 2.0f * nw;
                float ymax = cam.size / 2.0f * nh;
                return glm::ortho(-xmax, xmax, -ymax, ymax, 1e-5f, 1e2f);
            });

        auto c2w = camera_space_to_world_space(cam);
        auto w2c = inverse(c2w);

        return proj * w2c;
    }
}

