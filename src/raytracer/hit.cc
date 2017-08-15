#include "hit.hh"
#include "../math/constants.hh"

namespace rt::raytracer::hits
{
    namespace
    {
        using math::inf;
    }

    ray_type viewing_ray(object_hit_type const& a)
    {
        return a.match(
                [] (missed const& a) { return a.viewing; },
                [] (object const& a) { return a.shape_info.viewing; });
    }

    ray_type viewing_ray(shape_hit_type const& a)
    {
        return a.match([] (auto& a) { return a.viewing; });
    }

    float ray_extent(object_hit_type const& a)
    {
        return a.match(
                [] (missed const&) { return inf; },
                [] (object const& a) { return a.shape_info.ray_extent; });
    }

    float ray_extent(shape_hit_type const& a)
    {
        return a.match(
                [] (missed const&) { return inf; },
                [] (shape const& a) { return a.ray_extent; });
    }

    bool less_extent(object_hit_type const& a, object_hit_type const& b)
    {
        return ray_extent(a) < ray_extent(b);
    }

    bool less_extent(shape_hit_type const& a, shape_hit_type const& b)
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

    shape_hit_type extent_lesser_one(shape_hit_type a, shape_hit_type b)
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

