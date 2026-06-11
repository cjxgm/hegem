#pragma once
#include "../lib/glm/vec2.hxx"
#include "../math/constants.hxx"
#include "../math/modulus.hxx"
#include <vector>
#include <cmath>

namespace hegem::silo
{
    struct polar_vertex
    {
        float angle{};
        float length{};

        using cartesian_type = glm::vec2;

        polar_vertex() = default;
        polar_vertex(float angle, float length): angle{angle}, length{length} {}
        polar_vertex(cartesian_type pos, float rotation): polar_vertex{pos} { rotate_in_place(rotation); }
        polar_vertex(cartesian_type pos)
            : angle{std::atan2(pos.y, pos.x)}
            , length{std::hypot(pos.y, pos.x)}
        {}
        operator cartesian_type () const { return length * glm::vec2{std::cos(angle), std::sin(angle)}; }

        auto rotate(float a) const -> polar_vertex
        {
            auto result = *this;
            result.rotate_in_place(a);
            return result;
        }

        void rotate_in_place(float a)
        {
            angle += a;
            angle = math::modulus(angle + math::pi, 2.0f * math::pi) - math::pi;
        }
    };

    struct polar_path_cache_entry
    {
        polar_vertex::cartesian_type pos{};
        float angle_sum_so_far{};

        polar_path_cache_entry() = default;
        polar_path_cache_entry(polar_vertex::cartesian_type pos, float angle)
            : pos{pos}
            , angle_sum_so_far{angle}
        {}
    };

    using polar_path = std::vector<polar_vertex>;
    using polar_path_cache = std::vector<polar_path_cache_entry>;

    void update_polar_path_cache(polar_path const& path, polar_path_cache& cache);
    void update_polar_path_interpolation(polar_path const& src0, polar_path const& src1, float x, polar_path& dst);
    void move_polar_vertex_to(polar_path& path, polar_path_cache const& cache, int vertex_idx, polar_vertex::cartesian_type pos);
    void move_polar_vertex_locally_to(polar_path& path, polar_path_cache const& cache, int vertex_idx, polar_vertex::cartesian_type pos);
    auto edit_polar_path(polar_path& path, polar_path_cache const& cartesian_cache, bool active, bool readonly, glm::vec2 origin, float scaling, int* hovered_vertex_index=nullptr) -> bool;
}

