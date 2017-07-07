#include "../lib/glm/op/intersect.hh"
#include "../scene/shape.hh"
#include "intersect.hh"
#include <numeric>

namespace rt::raytracer
{
    namespace
    {
        using namespace scene;

        inline namespace intersect_shape_details
        {
            shape_hit_type intersect_shape(ray_type const& ray, shapes::sphere const& shape)
            {
                hits::point_type position;
                glm::vec3 normal;

                if (intersectRaySphere(
                            ray.origin, *ray.dir,
                            shape.center, shape.radius,
                            position, normal)) {
                    auto extent = distance(ray.origin, position);
                    return hits::shape{
                        ray,
                        extent,
                        1e-5f * extent,
                        position,
                        normal,
                    };
                } else {
                    return hits::missed{ray};
                }
            }

            shape_hit_type intersect_shape(ray_type const& ray, shapes::plane const& shape)
            {
                auto p = ray.origin - *shape.normal * shape.offset;
                auto pn = dot( p      , *shape.normal);
                auto dn = dot(*ray.dir, *shape.normal);

                if (pn * dn < 0) {
                    auto extent = pn / -dn;
                    return hits::shape{
                        ray,
                        extent,
                        1e-5f * extent,
                        ray.at(extent),
                        shape.normal,
                    };
                } else {
                    return hits::missed{ray};
                }
            }

            shape_hit_type intersect_shape(ray_type const& ray, shapes::mesh const& shape)
            {
                // TODO
                (void)shape;
                return hits::missed{ray};
            }
        }

        shape_hit_type intersect(ray_type const& ray, shape_type const& shape)
        {
            return shape.match([&] (auto& shape) {
                return intersect_shape(ray, shape);
            });
        }

        object_hit_type intersect(ray_type const& ray, cached_object const& obj)
        {
            return intersect(ray, obj.shape).match(
                [] (hits::missed m) -> object_hit_type { return m; },
                [&] (hits::shape shape_info) -> object_hit_type {
                    return hits::object{ obj.material_id, shape_info };
                });
        }
    }

    object_hit_type intersect(scene::scene_cache const& sc, ray_type const& ray)
    {
        return std::accumulate(
            begin(sc.objects), end(sc.objects),
            object_hit_type{hits::missed{ray}},
            [&] (auto hit, auto& obj) {
                return extent_lesser_one(hit, intersect(ray, obj));
            });
    }

    bool is_intersected_within(scene::scene_cache const& sc, ray_type const& ray, float extent)
    {
        return intersect(sc, ray).match(
            [] (hits::missed) { return false; },
            [&] (hits::object hit) {
                return (hit.shape_info.ray_extent < extent);
            });
    }
}

