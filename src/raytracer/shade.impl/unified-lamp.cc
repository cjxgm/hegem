#include "../../lib/glm/op/geom.hh"
#include "../../math/constants.hh"
#include "unified-lamp.hh"

namespace rt::raytracer::shading_details::unified_lamp_details
{
    namespace
    {
        namespace lamps = scene::lamps;
        using math::inf;
    }

    unified_lamp unify_lamp(scene::lamp_type const& lamp, hits::object const& hit)
    {
        return lamp.match(
            [&] (lamps::sun const& lamp) {
                return unified_lamp {
                    .received_radiance = lamp.color,
                    .towards_lamp = ray_type {
                        hit.shape_info.hit_point,
                        -*lamp.dir,
                    },
                    .distance_to_lamp = inf,
                };
            },
            [&] (lamps::omni const& lamp) {
                auto obj_to_lamp = lamp.center - hit.shape_info.hit_point;
                auto dist = length(obj_to_lamp) + 1;        // dist starts from 1
                return unified_lamp {
                    .received_radiance = lamp.color / (dist * dist),
                    .towards_lamp = ray_type {
                        hit.shape_info.hit_point,
                        obj_to_lamp,
                    },
                    .distance_to_lamp = dist - 1,            // distance starts from 0
                };
            });
    }
}

