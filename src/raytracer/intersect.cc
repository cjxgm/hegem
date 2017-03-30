#include "../lib/glm/op/intersect.hh"
#include "../scene/geometry.hh"
#include "intersect.hh"
#include <numeric>

namespace rt::raytracer
{
    namespace
    {
        using namespace scene;

        inline namespace intersect_geometry_details
        {
            geometry_hit_type intersect_geometry(ray_type const& ray, geometrys::sphere const& geom)
            {
                hits::point_type position;
                glm::vec3 normal;

                if (intersectRaySphere(
                            ray.origin, *ray.dir,
                            geom.center, geom.radius,
                            position, normal)) {
                    return hits::geometry{
                        ray,
                        distance(ray.origin, position),
                        position,
                        normal,
                    };
                } else {
                    return hits::missed{};
                }
            }

            geometry_hit_type intersect_geometry(ray_type const& ray, geometrys::line_segment const& geom)
            {
                // Raytracer DOES NOT render lines.
                // So this is always missed.
                return hits::missed{};
            }

            geometry_hit_type intersect_geometry(ray_type const& ray, geometrys::mesh const& geom)
            {
                // TODO
                return hits::missed{};
            }
        }

        inline namespace intersect_node_details
        {
            object_hit_type intersect_node(ray_type const& ray, nodes::object const& node)
            {
                return node.geometry.match([&] (auto& geom) {
                        return intersect_geometry(ray, geom);
                    }).match(
                        [] (hits::missed m) -> object_hit_type { return m; },
                        [&] (hits::geometry geom) -> object_hit_type {
                            return hits::object{ node.material_id, geom };
                        }
                    );
            }

            object_hit_type intersect_node(ray_type const& ray, nodes::group const& node)
            {
                return std::accumulate(
                        begin(node.nodes), end(node.nodes),
                        object_hit_type{hits::missed{}},
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

