#include "../lib/glm/mat3.hh"
#include "view.hh"

namespace rt::scene
{
    glm::mat3 view_type::screen_space_to_camera_plane_space() const
    {
        // aspect corrected normalized width and height
        auto nw = (size.x > size.y ? float(size.x) / size.y : 1.0f);
        auto nh = (size.y > size.x ? float(size.y) / size.x : 1.0f);

        float xmax = size.x - 1;
        float ymax = size.y - 1;

        return {
            2.0f*nw/xmax, 0            , 0,
            0           , -2.0f*nh/ymax, 0,
            -nw         , nh           , 1,
        };
    }
}

