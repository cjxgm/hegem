#include "../lib/glm/vec4.hh"
#include "../lib/glm/mat3.hh"
#include "../lib/glm/mat4.hh"
#include "../lib/glm/op/geom.hh"
#include "../lib/glm/op/trig.hh"
#include "../lib/glm/op/mat.hh"
#include "camera.hh"

namespace rt::scene::cameras
{
    namespace
    {
        // Compensate a graphical environment that does not have
        // abstractions over camera lenses.
        //
        // The raytracer assumes that the "lens" has a size.
        // The rasterizer thinks that the "lens" is just an infinitesimal pin hole.
        float sized_lens_to_pin_hole(float aspect_corrected_h, float yfov)
        {
            return aspect_corrected_h / glm::tan(yfov / 2.0f);
        }

        glm::mat4 zoffset(float offset)
        {
            return {
                glm::vec4{1, 0, 0, 0},
                glm::vec4{0, 1, 0, 0},
                glm::vec4{0, 0, 1, 0},
                glm::vec4{0, 0, offset, 1},
            };
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
    }

    glm::mat4 camera_space_to_world_space(camera_type const& cam)
    {
        return cam.match([&] (auto& cam_alt) {
            auto c2w_rot = camera_space_to_world_space_rotation_only(cam);
            auto extra_rotation = inverse(rotation(cam_alt.tt));
            return glm::mat4{
                glm::vec4{c2w_rot[0]    , 0},
                glm::vec4{c2w_rot[1]    , 0},
                glm::vec4{c2w_rot[2]    , 0},
                glm::vec4{cam_alt.center, 1},
            } * extra_rotation;
        });
    }

    glm::mat4 world_space_to_camera_space(camera_type const& cam)
    {
        auto c2w = camera_space_to_world_space(cam);
        auto w2c = inverse(c2w);
        return w2c;
    }

    glm::mat4 projection_of(camera_type const& cam, float aspect_ratio)
        // TODO: allow user to adjust far plane?
    {
        // aspect corrected normalized width and height
        auto nw = aspect_ratio > 1.0f ?        aspect_ratio : 1.0f;
        auto nh = aspect_ratio < 1.0f ? 1.0f / aspect_ratio : 1.0f;

        auto proj = cam.match(
            [=] (pin_hole const& cam) {
                float w = 2.0f * nw;
                float h = 2.0f * nh;
                auto s2p_len = sized_lens_to_pin_hole(nh, cam.fov);
                auto s2p = zoffset(-s2p_len);
                return glm::perspectiveFov(cam.fov, w, h, s2p_len, 1e2f) * s2p;
            },
            [=] (orthographic const& cam) {
                float xmax = cam.size / 2.0f * nw;
                float ymax = cam.size / 2.0f * nh;
                return glm::ortho(-xmax, xmax, -ymax, ymax, 1e-5f, 1e2f);
            });

        return proj;
    }

    glm::mat4 world_space_to_clip_space(camera_type const& cam, float aspect_ratio)
    {
        auto proj = projection_of(cam, aspect_ratio);
        auto w2c = world_space_to_camera_space(cam);
        return proj * w2c;
    }

    glm::vec3 apex_in_world_space(camera_type const& cam, float aspect_ratio)
    {
        // aspect corrected normalized height
        auto nh = aspect_ratio < 1.0f ? 1.0f / aspect_ratio : 1.0f;

        return cam.match(
            [=] (pin_hole const& cam) {
                auto c2w = camera_space_to_world_space(cam);
                auto forward = glm::vec3{c2w * glm::vec4{0.0f, 0.0f, -1.0f, 0.0f}};
                auto center  = glm::vec3{c2w * glm::vec4{0.0f, 0.0f,  0.0f, 1.0f}};
                auto offset = sized_lens_to_pin_hole(nh, cam.fov) * forward;
                return center - offset;
            },
            [=] (orthographic const& cam) {
                auto c2w = camera_space_to_world_space(cam);
                auto center  = glm::vec3{c2w * glm::vec4{0.0f, 0.0f,  0.0f, 1.0f}};
                return center;
            });
    }
}

