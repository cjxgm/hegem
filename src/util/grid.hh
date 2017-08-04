// Voxel grids
#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/glm/op/common.hh"
#include "../global/counter.hh"
#include <utility>      // for std::move
#include <vector>
#include <limits>

namespace rt::util
{
    namespace grid_details
    {
        using global::counter;

        static constexpr auto inf = std::numeric_limits<float>::infinity();

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

        private:
            face_trait face_trait_state;
            int subdivision;
            bound_type bound;
            cells_type cells;

            static constexpr int face_capacity() { return face_trait::capacity(); }
            auto face_minmax(face_id_type id) const { return face_trait_state.minmax(id); }

            auto cell_count() const { return subdivision * subdivision * subdivision; }
            int index_of(int x, int y, int z) const { return (z * subdivision + y) * subdivision + x; }

            glm::ivec3 world_pos_to_cell_pos(glm::vec3 p) const
            {
                auto model_pos = p - bound.min;
                return glm::ivec3{glm::floor(model_pos / cell_size())};
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
                for (int z=cell_from.z; z<=cell_from.z; z++) {
                    for (int y=cell_from.y; y<=cell_from.y; y++) {
                        for (int x=cell_from.x; x<=cell_from.x; x++) {
                            auto index = index_of(x, y, z);
                            cells[index].emplace_back(face);
                        }
                    }
                }
            }
        };
    }

    template <class FaceTrait>
    using grid = grid_details::grid<FaceTrait>;
}

