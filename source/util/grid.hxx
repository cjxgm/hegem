// Voxel grids
#pragma once
#include "../lib/glm/vec3.hxx"
#include "../lib/glm/op/common.hxx"
#include "../lib/glm/relational.hxx"
#include "../global/counter.hxx"
#include "../math/ray-aabb.hxx"
#include "../math/constants.hxx"
#include "../raytracer/ray.hxx"
#include "../raytracer/hit.hxx"
#include <utility>      // for std::move
#include <vector>
#include <limits>
#include <numeric>

namespace hegem::util
{
    namespace grid_details
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
        struct grid
        {
            using face_trait = FaceTrait;
            using face_id_type = typename face_trait::identifier_type;
            using face_soup_type = std::vector<face_id_type>;
            using cells_type = std::vector<face_soup_type>;
            using bound_type = bounding_box;

            grid(face_trait state, int subdivision, face_soup_type face_soup)
                : face_trait_state{std::move(state)}
                , subdivision{subdivision}
                , bound{bound_of(face_soup)}
                , cells(cell_count())
            {
                populate_cells(std::move(face_soup));
            }

            glm::vec3 cell_size() const
            {
                return (bound.max - bound.min) / float(subdivision);
            }

            template <class F>
            void each(F&& f) const
            {
                glm::ivec3 cell_pos{};
                for (auto& cell: cells) {
                    f(cell, cell_pos);
                    cell_pos.x++;
                    if (cell_pos.x == subdivision) { cell_pos.x = 0; cell_pos.y++; }
                    if (cell_pos.y == subdivision) { cell_pos.y = 0; cell_pos.z++; }
                }
            }

            glm::vec3 cell_center_pos_to_world_pos(glm::ivec3 p) const
            {
                return (glm::vec3{p} + glm::vec3{0.5f}) * cell_size() + bound.min;
            }

            shape_hit_type intersect(ray_type const& ray) const
            {
                counter.ix_grid++;
                auto extent = math::ray_intersect_aabb_from_far(ray, bound.min, bound.max);
                if (extent < inf) {
                    return intersect(ray, extent);
                } else {
                    return hits::missed{ray};
                }
            }

        private:
            face_trait face_trait_state;
            int subdivision;
            bound_type bound;
            cells_type cells;

            static constexpr int face_capacity() { return face_trait::capacity(); }
            auto face_minmax(face_id_type id) const { return face_trait_state.minmax(id); }

            auto cell_count() const { return subdivision * subdivision * subdivision; }
            int index_of(int x, int y, int z) const { return (z * subdivision + y) * subdivision + x; }
            int index_of(glm::ivec3 cell_pos) const { return index_of(cell_pos.x, cell_pos.y, cell_pos.z); }

            glm::ivec3 world_pos_to_cell_pos(glm::vec3 p) const
            {
                auto model_pos = p - bound.min;
                auto cell_pos = glm::ivec3{glm::floor(model_pos / cell_size())};
                return glm::clamp(cell_pos, glm::ivec3{0}, glm::ivec3{subdivision-1});
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

            void populate_cells(face_soup_type faces)
            {
                for (auto face: faces) add_face(face);
            }

            void add_face(face_id_type face)
            {
                auto [min_pos, max_pos] = face_minmax(face);
                auto cell_from = world_pos_to_cell_pos(min_pos);
                auto cell_to   = world_pos_to_cell_pos(max_pos);

                for (int z=cell_from.z; z<=cell_to.z; z++) {
                    for (int y=cell_from.y; y<=cell_to.y; y++) {
                        for (int x=cell_from.x; x<=cell_to.x; x++) {
                            auto index = index_of(x, y, z);
                            cells[index].emplace_back(face);
                        }
                    }
                }
            }

            template <class T>
            static bool in_bound(T const& pos, T const& bmin, T const& bmax)
            {
                if (any(lessThan(pos, bmin))) return false;
                if (any(greaterThan(pos, bmax))) return false;
                return true;
            }

            bool in_bound(glm::vec3 const& pos) const
            {
                return in_bound(pos, bound.min, bound.max);
            }

            shape_hit_type intersect(ray_type const& ray, float hit_extent) const
            {
                auto local_pos = ray.origin - bound.min;
                if (!in_bound(ray.origin)) local_pos += *ray.dir * hit_extent;

                auto cs = cell_size();
                auto cell_pos = glm::ivec3{local_pos / cs};
                if (cell_pos[0] < 0) cell_pos[0] = 0;
                if (cell_pos[1] < 0) cell_pos[1] = 0;
                if (cell_pos[2] < 0) cell_pos[2] = 0;
                if (cell_pos[0] >= subdivision) cell_pos[0] = subdivision - 1;
                if (cell_pos[1] >= subdivision) cell_pos[1] = subdivision - 1;
                if (cell_pos[2] >= subdivision) cell_pos[2] = subdivision - 1;

                auto delta = cs / *ray.dir;
                auto extent = (glm::vec3{cell_pos} * cs - local_pos + cs) / *ray.dir;
                auto step = glm::ivec3{1};

                if (ray.dir->x < 0) {
                    delta[0] = -delta[0];
                    step[0] = -1;
                    extent[0] += delta[0];
                }
                if (ray.dir->y < 0) {
                    delta[1] = -delta[1];
                    step[1] = -1;
                    extent[1] += delta[1];
                }
                if (ray.dir->z < 0) {
                    delta[2] = -delta[2];
                    step[2] = -1;
                    extent[2] += delta[2];
                }

                auto cell_pos_min = glm::ivec3{0};
                auto cell_pos_max = glm::ivec3{subdivision - 1};
                while (in_bound(cell_pos, cell_pos_min, cell_pos_max)) {
                    auto index = index_of(cell_pos);
                    auto& cell = cells[index];

                    auto hit = intersect(cell, ray);
                    if (!hit.template is<hits::missed>()) {
                        return hit;
                    }

                    if (extent.x < extent.y && extent.x < extent.z) {
                        extent[0] += delta[0];
                        cell_pos[0] += step[0];
                    }
                    else if (extent.y < extent.z) {
                        extent[1] += delta[1];
                        cell_pos[1] += step[1];
                    }
                    else {
                        extent[2] += delta[2];
                        cell_pos[2] += step[2];
                    }
                }

                return hits::missed{ray};
            }

            shape_hit_type intersect(face_id_type face, ray_type const& ray) const
            {
                return face_trait_state.intersect(face, ray);
            }

            shape_hit_type intersect(face_soup_type const& faces, ray_type const& ray) const
            {
                counter.ix_grid_face += faces.size();
                return std::accumulate(
                    begin(faces), end(faces),
                    shape_hit_type{hits::missed{ray}},
                    [&] (auto hit, auto& obj) {
                        return extent_lesser_one(hit, intersect(obj, ray));
                    });
            }
        };
    }

    template <class FaceTrait>
    using grid = grid_details::grid<FaceTrait>;
}

