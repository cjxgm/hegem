#include "view.hh"

namespace rt::scene
{
    glm::mat4 view_type::transformation() const
    {
        auto camera_transform = camera_.match([] (auto& cam) {
            return cam.transformation();
        });

        // aspect corrected normalized width and height
        auto nw = (size_.x > size_.y ? float(size_.x) / size_.y : 1.0f);
        auto nh = (size_.y > size_.x ? float(size_.y) / size_.x : 1.0f);

        float xmax = size_.x - 1;
        float ymax = size_.y - 1;

        glm::mat4 view_transform{
            2.0f*nw/xmax, 0            , 0, 0,
            0           , -2.0f*nh/ymax, 0, 0,
            0           , 0            , 1, 0,
            -nw         , nh           , 0, 1,
        };

        return camera_transform * view_transform;
    }

    glm::mat3 view_type::camera_rotation() const
    {
        return camera_.match([] (auto& cam) {
            return cam.rotation();
        });
    }
}

