#include "../swing/primitive.hxx"
#include "../swing/geometry.hxx"
#include "../swing/list.hxx"
#include "../math/direction.hxx"
#include "../math/local-space.hxx"
#include "../math/constants.hxx"
#include "../lib/glm/mat3.hxx"
#include "invoke.impl/model.hxx"
#include "mesh.hxx"

namespace rt::sk
{
    inline namespace mesh
    {
        namespace
        {
            auto barycenter_of(swing::hege_type* first) -> glm::vec3
            {
                if (first == nullptr) return {};

                glm::vec3 bary;
                int count{};
                for (auto& h: swing::list::iterate(first)) {
                    bary += h.start->pos;
                    count++;
                }

                return (count == 0 ? bary : bary / float(count));
            }

            auto vertex_mesh(swing::vert_type* v) -> scene::shapes::mesh
            {
                constexpr auto radius = 0.05f;
                constexpr glm::vec3 positions[] = {
                    { 0.0f, -radius, 0.0f },    // 0
                    { +radius, 0.0f, 0.0f },    // 1
                    { 0.0f, 0.0f, -radius },    // 2
                    { -radius, 0.0f, 0.0f },    // 3
                    { 0.0f, 0.0f, +radius },    // 4
                    { 0.0f, +radius, 0.0f },    // 5
                };
                constexpr int faces[][3] = {
                    { 0, 2, 1 },
                    { 0, 3, 2 },
                    { 0, 4, 3 },
                    { 0, 1, 4 },
                    { 1, 2, 5 },
                    { 2, 3, 5 },
                    { 3, 4, 5 },
                    { 4, 1, 5 },
                };

                scene::shapes::mesh m;
                for (auto& ids: faces) {
                    auto p0 = positions[ids[0]];
                    auto p1 = positions[ids[1]];
                    auto p2 = positions[ids[2]];
                    auto n = math::direction_type{cross(p1-p0, p2-p0)};
                    auto first = int(m.verts.size());
                    m.verts.push_back({ v->pos + p0, n });
                    m.verts.push_back({ v->pos + p1, n });
                    m.verts.push_back({ v->pos + p2, n });
                    m.faces.emplace_back(first, first+1, first+2);
                }

                return m;
            }

            auto face_mesh(swing::face_type* f) -> scene::shapes::mesh
            {
                if (f->boundary->any_hege == nullptr)
                    return {};

                constexpr auto radius = 0.03f;
                constexpr auto height = 0.30f;
                constexpr auto hat_spread = 0.02f;
                constexpr auto hat_height = 0.10f;
                constexpr auto hat_sliding = 0.05f;
                constexpr glm::vec3 positions[] = {
                    { +radius, 0.0f, 0.0f },    // 0
                    { 0.0f, +radius, 0.0f },    // 1
                    { -radius, 0.0f, 0.0f },    // 2
                    { 0.0f, -radius, 0.0f },    // 3

                    { +radius, 0.0f, height },  // 4
                    { 0.0f, +radius, height },  // 5
                    { -radius, 0.0f, height },  // 6
                    { 0.0f, -radius, height },  // 7

                    { +radius+hat_spread, 0.0f, height - hat_sliding }, // 8
                    { 0.0f, +radius+hat_spread, height - hat_sliding }, // 9
                    { -radius-hat_spread, 0.0f, height - hat_sliding }, // 10
                    { 0.0f, -radius-hat_spread, height - hat_sliding }, // 11

                    { 0.0f, 0.0f, height + hat_height },    // 12
                };
                constexpr int faces[][3] = {
                    { 0, 1, 4 },
                    { 1, 2, 5 },
                    { 2, 3, 6 },
                    { 3, 0, 7 },

                    { 5, 4, 1 },
                    { 6, 5, 2 },
                    { 7, 6, 3 },
                    { 4, 7, 0 },

                    { 4, 5,  8 },
                    { 5, 6,  9 },
                    { 6, 7, 10 },
                    { 7, 4, 11 },

                    {  9,  8, 5 },
                    { 10,  9, 6 },
                    { 11, 10, 7 },
                    {  8, 11, 4 },

                    {  8,  9, 12 },
                    {  9, 10, 12 },
                    { 10, 11, 12 },
                    { 11,  8, 12 },
                };

                auto nf = swing::normal(f->boundary->any_hege);
                auto local_axis = math::local_space(nf);
                auto median = barycenter_of(f->boundary->any_hege);

                scene::shapes::mesh m;
                for (auto& ids: faces) {
                    auto p0 = local_axis * positions[ids[0]];
                    auto p1 = local_axis * positions[ids[1]];
                    auto p2 = local_axis * positions[ids[2]];
                    auto n = math::direction_type{cross(p1-p0, p2-p0)};
                    auto first = int(m.verts.size());
                    m.verts.push_back({ median + p0, n });
                    m.verts.push_back({ median + p1, n });
                    m.verts.push_back({ median + p2, n });
                    m.faces.emplace_back(first, first+1, first+2);
                }

                return m;
            }
        }

        auto build_selection_mesh(op::invoke_impl::model const& md) -> scene::shapes::mesh
        {
            scene::shapes::mesh m;
            for (auto vert: md.vert_selection)
                extend(m, vertex_mesh(vert));
            for (auto face: md.face_selection)
                extend(m, face_mesh(face));
            return m;
        }
    }
}

