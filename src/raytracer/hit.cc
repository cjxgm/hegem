#include "hit.hh"
#include <limits>

namespace rt::raytracer::hits
{
    namespace
    {
        static constexpr auto inf = std::numeric_limits<float>::infinity();
    }

    ray_type viewing_ray(object_hit_type const& a)
    {
        return a.match(
                [] (missed const& a) { return a.viewing; },
                [] (object const& a) { return a.shape_info.viewing; });
    }

    float ray_extent(object_hit_type const& a)
    {
        return a.match(
                [] (missed const&) { return inf; },
                [] (object const& a) { return a.shape_info.ray_extent; });
    }

    bool less_extent(object_hit_type const& a, object_hit_type const& b)
    {
        return ray_extent(a) < ray_extent(b);
    }

    object_hit_type extent_lesser_one(object_hit_type a, object_hit_type b)
    {
        if (less_extent(a, b)) {
            return a;
        } else {
            return b;
        }
    }

    ray_type biased_ray(ray_type ray, shape shape_info)
    {
        return {
            ray.origin + *shape_info.normal * shape_info.ray_max_error,
            ray.dir,
        };
    }
}

namespace rt::image::image_impl
{
    template struct image<raytracer::object_hit_type>;
}

