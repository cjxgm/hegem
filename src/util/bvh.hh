// Bounding Volume Hierarchy
#pragma once
#include "../lib/mapbox/variant.hh"
#include "../lib/glm/vec3.hh"
#include "../lib/glm/relational.hh"
#include "../lib/glm/op/common.hh"
#include "../lib/glm/op/intersect.hh"
#include "../raytracer/ray.hh"
#include "../raytracer/hit.hh"
#include "../global/counter.hh"
#include "../math/ray-aabb.hh"
#include "../math/constants.hh"
#include <utility>      // for std::move
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <iostream>     // DEBUG TODO: use journal?

namespace rt::util
{
    namespace bvh_details
    {
        using global::counter;
        using raytracer::shape_hit_type;
        using raytracer::ray_type;
        using math::inf;
        namespace hits = raytracer::hits;

        struct bounding_box
        {
            glm::vec3 min;
            glm::vec3 max;
        };

        template <class FaceTrait>
        struct bvh
        {
            struct node;
            using face_trait = FaceTrait;
            using face_id_type = typename face_trait::identifier_type;
            using face_soup_type = std::vector<face_id_type>;
            using node_soup_type = std::vector<node>;
            using partitioned_face_soup_type = std::vector<face_soup_type>;
            using bound_type = bounding_box;
            using storage_type = mapbox::util::variant<face_soup_type, node_soup_type>;

            struct node
            {
                bound_type bound;
                storage_type storage;
                int face_count;
            };

            bvh(face_trait state, face_soup_type face_soup)
                : face_trait_state{std::move(state)}
                , root{build_node(std::move(face_soup))}
            {}

            shape_hit_type intersect(ray_type const& ray) const
            {
                counter.ray_bvh_incoming++;
                return intersect(root, ray);
            }

            void debug() const { debug(root, 0); }

        private:
            face_trait face_trait_state;
            node root;

            static constexpr int face_capacity() { return face_trait::capacity(); }
            auto face_minmax(face_id_type id) const { return face_trait_state.minmax(id); }
            glm::vec3 face_pivot(face_id_type id) const { return face_trait_state.pivot(id); }

            node build_node(face_soup_type faces) const
            {
                auto bound = bound_of(faces);
                auto partition_pivot = center_of_mass(faces);
                auto storage = build_storage(std::move(faces), partition_pivot);
                auto face_count = count_face(storage);
                return {
                    std::move(bound),
                    std::move(storage),
                    face_count,
                };
            }

            storage_type build_storage(face_soup_type faces, glm::vec3 const& partition_pivot) const
            {
                if (faces.size() == 0)
                    throw std::logic_error{"assert(faces.size() > 0)"};

                if (static_cast<int>(faces.size()) < face_capacity())
                    return std::move(faces);

                auto face_soups = partition(std::move(faces), partition_pivot);
                if (face_soups.size() == 1)
                    return std::move(face_soups[0]);

                node_soup_type nodes;
                for (auto& faces: face_soups)
                    nodes.emplace_back(build_node(std::move(faces)));

                return nodes;
            }

            bound_type bound_of(face_soup_type const& faces) const
            {
                auto min_pos = glm::vec3{+inf};
                auto max_pos = glm::vec3{-inf};
                for (auto face: faces) {
                    auto [face_min_pos, face_max_pos] = face_minmax(face);
                    min_pos = glm::min(min_pos, face_min_pos);
                    max_pos = glm::max(max_pos, face_max_pos);
                }

                return {
                    min_pos,
                    max_pos,
                };
            }

            glm::vec3 center_of_mass(face_soup_type const& faces) const
            {
                glm::vec3 sum{};
                for (auto face: faces)
                    sum += face_pivot(face);

                return sum / float(faces.size());
            }

            partitioned_face_soup_type partition(face_soup_type faces, glm::vec3 const& partition_pivot) const
            {
                partitioned_face_soup_type face_soups(8);
                for (auto face: faces) {
                    auto index = partition_index_of(face, partition_pivot);
                    face_soups[index].emplace_back(face);
                }

                face_soups.erase(
                    std::remove_if(
                        begin(face_soups),
                        end(face_soups),
                        [] (auto& faces) { return faces.empty(); }),
                    end(face_soups));
                return face_soups;
            }

            int partition_index_of(face_id_type face, glm::vec3 const& partition_pivot) const
            {
                auto pivot = face_pivot(face);
                auto address = glm::ivec3{greaterThan(pivot, partition_pivot)};
                auto index = (address[2] << 2) | (address[1] << 1) | (address[0] << 0);
                return index;
            }

            shape_hit_type intersect(node const& n, ray_type const& ray) const
            {
                {
                    float extent = math::ray_intersect_aabb_from_far(ray, n.bound.min, n.bound.max);
                    if (extent == inf) {
                        return hits::missed{ray};
                    }
                }

                n.storage.match(
                    [] (face_soup_type const& faces) {
                        counter.ix += faces.size();
                        counter.ix_bvh += faces.size();
                        counter.ix_bvh_face += faces.size();
                    },
                    [&] (node_soup_type const& nodes) {
                        counter.ix += nodes.size();
                        counter.ix_bvh += nodes.size();
                        counter.ix_bvh_bound += nodes.size();
                    });

                return n.storage.match(
                    [&] (auto& soup) {
                        return std::accumulate(
                            begin(soup), end(soup),
                            shape_hit_type{hits::missed{ray}},
                            [&] (auto hit, auto& obj) {
                                return extent_lesser_one(hit, intersect(obj, ray));
                            });
                    });
            }

            shape_hit_type intersect(face_id_type face, ray_type const& ray) const
            {
                return face_trait_state.intersect(face, ray);
            }

            void debug(node const& n, int indent) const
            {
                indent += 4;
                n.storage.match(
                    [] (face_soup_type const& faces) {
                        std::cerr << "# " << faces.size() << "\n";
                    },
                    [&] (node_soup_type const& nodes) {
                        std::cerr << "-\n";
                        int nid = 0;
                        for (auto& n: nodes) {
                            for (int i=0; i<indent; i++) std::cerr << " ";
                            std::cerr << "[" << nid++ << "] ";
                            debug(n, indent);
                        }
                    });
            }

            int count_face(storage_type const& storage) const
            {
                return storage.match(
                    [] (face_soup_type const& faces) {
                        return faces.size();
                    },
                    [&] (node_soup_type const& nodes) {
                        int sum = 0;
                        for (auto& n: nodes)
                            sum += n.face_count;
                        return sum;
                    });
            }
        };
    }

    template <class FaceTrait>
    using bvh = bvh_details::bvh<FaceTrait>;
}

