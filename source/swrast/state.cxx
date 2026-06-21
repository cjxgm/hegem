#include "../lib/glm/mat4.hxx"
#include "state.hxx"
#include <array>

namespace hegem::swrast::state_details
{
    inline namespace
    {
        using triangle_type = std::array<point_type, 3>;
        using triangle_mesh_type = std::vector<triangle_type>;

        auto collect_triangles(scene::scene_cache const& sc, glm::mat4 const& proj_view) -> triangle_mesh_type
        {
            triangle_mesh_type tm;
            for (auto& obj: sc.objects) {
                if (!obj.shape.is<scene::shapes::mesh>()) continue;
                auto m = obj.shape.get<scene::shapes::mesh>();

                // To NDC space
                auto proj_view_model = proj_view * obj.model_to_world;
                for (auto& vert: m.verts) {
                    auto homo = proj_view_model * glm::vec4{vert.position, 1.0f};
                    vert.position = point_type{homo} / homo.w * 0.5f + 0.5f;
                    vert.position.y = 1.0 - vert.position.y;    // invert y to correct the up-side down image
                }

                // Cull and collect
                for (auto& face: m.faces) {
                    auto p0 = m.verts[face.vert_ids[0]].position;
                    auto p1 = m.verts[face.vert_ids[1]].position;
                    auto p2 = m.verts[face.vert_ids[2]].position;

                    // Cull by near plane
                    if (p0.z < 0.0f || p1.z < 0.0f || p2.z < 0.0f) continue;

                    tm.emplace_back(triangle_type{p0, p1, p2});
                }
            }
            return tm;
        }
    }

    state::state(scene_type const& s, view_type view)
        : size{view.size}
    {
        auto aspect_ratio = float(view.size.x) / float(view.size.y);
        auto w2c = world_space_to_clip_space(view.camera, aspect_ratio);
        auto m = collect_triangles(s.cache, w2c);

        // background marker
        polys.emplace_back(point_type{0.0f, 0.0f, 1.0f}, point_type{1.0f, 0.0f, 1.0f}, point_type{0.0f, 1.0f, 1.0f});

        auto add_edge = [&] (int id, screen_coord_type p0, screen_coord_type p1) {
            if (p0.y == p1.y) return;
            if (p0.y > p1.y) std::swap(p0, p1); // ensure p0 is lower and p1 is upper.
            edges.emplace(id, p0, p1);
        };

        auto to_screen_space = [&] (point_type p) -> point_type {
            return point_type{glm::vec2{p} * glm::vec2{size}, p.z};
        };

        auto quantize = [&] (point_type p) -> screen_coord_type {
            return screen_coord_type{floor(p)};
        };

        int x_front = 1;
        int y_front = 1;
        int x_back = size.x - 2;
        int y_back = size.y - 2;
        for (auto& tri: m) {
            auto p0 = to_screen_space(tri[0]);
            auto p1 = to_screen_space(tri[1]);
            auto p2 = to_screen_space(tri[2]);

            auto q0 = quantize(p0);
            auto q1 = quantize(p1);
            auto q2 = quantize(p2);

            x_front = std::min(std::min(x_front, q0.x), std::min(q1.x, q2.x));
            x_back  = std::max(std::max(x_back,  q0.x), std::max(q1.x, q2.x));
            y_front = std::min(std::min(y_front, q0.y), std::min(q1.y, q2.y));

            auto id = int(polys.size());
            polys.emplace_back(p0, p1, p2);

            add_edge(id, q0, q1);
            add_edge(id, q1, q2);
            add_edge(id, q2, q0);
        }

        // enlarge raster region by 1 pixel along each direction
        x_front--;
        y_front--;
        x_back++;
        y_back++;

        // make x_back to x_last, y_back to y_last
        x_back++;
        y_back++;

        // left and right marker edge
        edges.emplace(0, screen_coord_type{x_front, y_front}, screen_coord_type{x_front, y_back});
        edges.emplace(0, screen_coord_type{x_back,  y_front}, screen_coord_type{x_back,  y_back});
    }
}

namespace hegem::swrast::state_details
{
    edge_descriptor::edge_descriptor(polygon_id_type id, screen_coord_type lower, screen_coord_type upper)
        : x{lower.x}
        , dx{upper.x - lower.x, upper.y - lower.y}
        , y_front{lower.y}
        , y_back{upper.y - 1}
        , id{id}
    {}
}

namespace hegem::swrast::state_details
{
    inline namespace
    {
        auto yzx(point_type a) { return point_type{a.y, a.z, a.x}; }

        template <int N>
        auto normal_of(
            point_type const (&points)[N],
            float eps=1e-1f,
            direction_type fallback = point_type{0, 0, -1}
        ) -> direction_type
        {
            // Use Newell's method to calculate the normal.
            // It works for concave polygons and near-parallel edges.
            //
            // Tampieri, Filippo. 1992. "Newell's Method for Computing
            // the Plane Equation.". Graphics Gems III 231–232. Academic Press.
            point_type normal;
            for (int i=0; i<N; i++) {
                auto u = points[i == 0 ? N-1 : i-1];
                auto v = points[i];
                auto u0 = yzx(u);
                auto v0 = yzx(v);
                auto u1 = yzx(u0);
                auto v1 = yzx(v0);
                normal += (u0 - v0) * (u1 + v1);
            }

            if (dot(normal, normal) <= eps*eps) return fallback;
            return normal;
        }
    }

    polygon_descriptor::polygon_descriptor(point_type p0, point_type p1, point_type p2)
        : normal{normal_of((point_type[]){p0, p1, p2})}
        , d{dot(*normal, p0)}
    {}

    auto polygon_descriptor::z_at(point_type p) const -> float
    {
        return (d - p.x * normal->x - p.y * normal->y) / normal->z;
    }
}

