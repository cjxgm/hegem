#include "../lib/glm/op/intersect.hh"
#include "../scene/shape.hh"
#include "../global/counter.hh"
#include "intersect.hh"
#include <numeric>
#include <iostream>

namespace rt::raytracer
{
    namespace
    {
        using namespace scene;
        using hits::direction_type;
        using global::counter;

        glm::vec3 operator * (glm::mat4 const& xform, glm::vec3 const& p)
        {
            return {xform * glm::vec4{p, 1.0f}};
        }

        direction_type operator * (glm::mat4 const& xform, direction_type const& d)
        {
            return {xform * glm::vec4{*d, 0.0f}};
        }

        ray_type operator * (glm::mat4 const& xform, ray_type const& ray)
        {
            return {
                xform * ray.origin,
                xform * ray.dir,
            };
        }

        inline namespace intersect_shape_details
        {
            shape_hit_type intersect_shape(ray_type const& ray, shapes::sphere const& shape, optional_mesh_bvh_type const& /*opt_bvh*/)
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

            shape_hit_type intersect_shape(ray_type const& ray, shapes::plane const& shape, optional_mesh_bvh_type const& /*opt_bvh*/)
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

            shape_hit_type intersect_shape(ray_type const& ray, shapes::mesh const& /*shape*/, optional_mesh_bvh_type const& opt_bvh)
            {
                if (!opt_bvh) {
                    std::cerr << "logic error: BVH not available\n";
                    std::abort();
                }
                return opt_bvh->intersect(ray);
            }

            shape_hit_type intersect_shape(ray_type const& ray, shapes::voxel const& shape, optional_mesh_bvh_type const& /*opt_bvh*/)
            {
                return shape.voxelized.intersect(ray);
            }
        }

        shape_hit_type intersect(ray_type const& ray, shape_type const& shape, optional_mesh_bvh_type const& opt_bvh)
        {
            counter.ix++;
            return shape.match([&] (auto& shape) {
                return intersect_shape(ray, shape, opt_bvh);
            });
        }

        object_hit_type intersect(ray_type const& ray, cached_object const& obj)
        {
            auto model_ray = obj.world_to_model * ray;
            return intersect(model_ray, obj.shape, obj.opt_bvh).match(
                [&] (hits::missed m) -> object_hit_type {
                    return hits::missed{ ray };
                },
                [&] (hits::shape shape_info) -> object_hit_type {
                    auto extent_scale = length(glm::vec3{obj.model_to_world * glm::vec4{*model_ray.dir, 0.0f}});
                    return hits::object{
                        obj.material_id,
                        hits::shape {
                            ray,
                            shape_info.ray_extent * extent_scale,
                            shape_info.ray_max_error * extent_scale,
                            obj.model_to_world * shape_info.hit_point,
                            transpose(obj.world_to_model) * shape_info.normal,
                        },
                    };
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

