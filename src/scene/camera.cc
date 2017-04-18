#include "../lib/glm/vec4.hh"
#include "../lib/glm/mat3.hh"
#include "../lib/glm/mat4.hh"
#include "../lib/glm/op/geom.hh"
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
}

