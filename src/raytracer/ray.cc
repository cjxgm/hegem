#include "../lib/glm/vec4.hh"
#include "../lib/glm/mat4.hh"
#include "ray.hh"
#include <cmath>

namespace rt::raytracer::ray_details
{
    namespace
    {
        namespace cameras = scene::cameras;

        struct camera_ray_generator
        {
            position_type const& p;
            glm::mat4 c2w;

            template <class Camera>
            ray_type operator () (Camera const& cam)
            {
                return impl(cam);
            }

        private:
            ray_type impl(cameras::orthographic const& cam)
            {
                auto half = cam.size / 2.0f;
                auto origin       = c2w * glm::vec4{p * half,  0, 1};
                auto extent_point = c2w * glm::vec4{p * half, -1, 1};
                return {
                    origin.xyz(),
                    (extent_point - origin).xyz(),
                };
            }

            ray_type impl(cameras::pin_hole const& cam)
            {
                auto tan_half = std::tan(cam.fov / 2.0f);
                auto s = 1 + tan_half;
                auto origin       = c2w * glm::vec4{p  ,  0, 1};
                auto extent_point = c2w * glm::vec4{p*s, -1, 1};
                return {
                    origin.xyz(),
                    (extent_point - origin).xyz(),
                };
            }
        };
    }

    ray_type camera_ray_from_camera_plane(position_type const& p, camera_type const& cam)
    {
        camera_ray_generator gen{p, scene::camera_space_to_world_space(cam)};
        return cam.match(gen);
    }
}

