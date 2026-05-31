#include "../lib/glm/vec4.hh"
#include "../lib/glm/mat4.hh"
#include "../lib/glm/op/trig.hh"
#include "ray.hh"

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
                    glm::vec3{origin},
                    glm::vec3{extent_point - origin},
                };
            }

            ray_type impl(cameras::pin_hole const& cam)
            {
                auto t = glm::tan(cam.fov / 2.0f);
                auto origin = c2w * glm::vec4{p  ,  0, 1};
                auto dir    = c2w * glm::vec4{p*t, -1, 0};
                return {
                    glm::vec3{origin},
                    glm::vec3{dir},
                };
            }
        };
    }

    ray_type camera_ray_from_camera_plane(position_type const& p, camera_type const& cam)
    {
        camera_ray_generator gen{p, scene::camera_space_to_world_space(cam)};
        return apply_visitor(gen, cam);
    }
}

