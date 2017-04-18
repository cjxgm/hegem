#include "../lib/glm/mat3.hh"
#include "view.hh"

namespace rt::scene
{
    glm::mat3 view_type::screen_space_to_camera_plane_space() const
    {
        // aspect corrected normalized width and height
        auto nw = (size_.x > size_.y ? float(size_.x) / size_.y : 1.0f);
        auto nh = (size_.y > size_.x ? float(size_.y) / size_.x : 1.0f);

        float xmax = size_.x - 1;
        float ymax = size_.y - 1;

        return {
            2.0f*nw/xmax, 0            , 0,
            0           , -2.0f*nh/ymax, 0,
            -nw         , nh           , 1,
        };
    }
}

