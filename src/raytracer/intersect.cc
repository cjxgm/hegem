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
            shape_hit_type intersect_shape(ray_type const& ray, shapes::sphere const& shape_info)
            {
                hits::point_type position;
                glm::vec3 normal;

                if (intersectRaySphere(
                            ray.origin, *ray.dir,
                            shape_info.center, shape_info.radius,
                            position, normal)) {
                    return hits::shape{
                        ray,
                        distance(ray.origin, position),
                        position,
                        normal,
                    };
                } else {
                    return hits::missed{ray};
                }
            }

            shape_hit_type intersect_shape(ray_type const& ray, shapes::line_segment const& shape_info)
            {
                // Raytracer DOES NOT render lines.
                // So this is always missed.
                return hits::missed{ray};
            }

            shape_hit_type intersect_shape(ray_type const& ray, shapes::mesh const& shape_info)
            {
                // TODO
                return hits::missed{ray};
            }
        }

        inline namespace intersect_node_details
        {
            object_hit_type intersect_node(ray_type const& ray, nodes::object const& node)
            {
                return node.shape.match([&] (auto& shape_info) {
                        return intersect_shape(ray, shape_info);
                    }).match(
                        [] (hits::missed m) -> object_hit_type { return m; },
                        [&] (hits::shape shape_info) -> object_hit_type {
                            return hits::object{ node.material_id, shape_info };
                        }
                    );
            }

            object_hit_type intersect_node(ray_type const& ray, nodes::group const& node)
            {
                return std::accumulate(
                        begin(node.nodes), end(node.nodes),
                        object_hit_type{hits::missed{ray}},
                        [&] (auto hit, auto& node) {
                            return extent_lesser_one(hit, intersect(node, ray));
                        });
            }
        }
    }

    object_hit_type intersect(scene::node_type const& node, ray_type const& ray)
    {
        return node.match([&] (auto& node) {
            return intersect_node(ray, node);
        });
    }
}

