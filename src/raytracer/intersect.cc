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

        inline namespace intersect_node_details
        {
            object_hit_type intersect_node(ray_type const& ray, node_type const& node);

            object_hit_type intersect_node_impl(ray_type const& ray, nodes::object const& node)
            {
                return node.shape.match([&] (auto& shape) {
                        return intersect_shape(ray, shape);
                    }).match(
                        [] (hits::missed m) -> object_hit_type { return m; },
                        [&] (hits::shape shape_info) -> object_hit_type {
                            return hits::object{ node.material_id, shape_info };
                        }
                    );
            }

            object_hit_type intersect_node_impl(ray_type const& ray, nodes::group const& node)
            {
                return std::accumulate(
                        begin(node.nodes), end(node.nodes),
                        object_hit_type{hits::missed{ray}},
                        [&] (auto hit, auto& node) {
                            return extent_lesser_one(hit, intersect_node(ray, node));
                        });
            }

            object_hit_type intersect_node_impl(ray_type const& ray, nodes::xform const& node)
            {
                return intersect_node(ray, node.node);
            }

            object_hit_type intersect_node(ray_type const& ray, node_type const& node)
            {
                return node.match([&] (auto& node) {
                    return intersect_node_impl(ray, node);
                });
            }
        }
    }

    object_hit_type intersect(scene::node_type const& node, ray_type const& ray)
    {
        return intersect_node(ray, node);
    }

    bool is_intersected_within(scene::node_type const& node, ray_type const& ray, float extent)
    {
        return intersect(node, ray).match(
            [] (hits::missed) { return false; },
            [&] (hits::object hit) {
                return (hit.shape_info.ray_extent < extent);
            });
    }
}

