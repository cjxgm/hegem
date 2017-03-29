#include "hit.hh"

namespace rt::raytracer::hits
{
    bool less_extent(object_hit_type const& a, object_hit_type const& b)
    {
        return a.match(
                [] (missed) { return false; },
                [&] (object const& a) {
                    return b.match(
                            [] (missed) { return true; },
                            [&] (object const& b) {
                                return (a.geom.ray_extent < b.geom.ray_extent);
                            });
                });
    }

    object_hit_type extent_lesser_one(object_hit_type a, object_hit_type b)
    {
        if (less_extent(a, b)) {
            return a;
        } else {
            return b;
        }
    }
}

