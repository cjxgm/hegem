#include "../../lib/glm/op/geom.hxx"
#include "../../lib/std/any.hxx"
#include "../../util/span.hxx"
#include "../../math/direction.hxx"
#include "../../swing/list.hxx"
#include "../../swing/iteration.hxx"
#include "../../swing/geometry.hxx"
#include "../op.hxx"
#include "model.hxx"
#include "select.hxx"
#include "util.hxx"
#include <utility>      // for std::move

namespace hegem::sk::op::invoke_impl
{
    namespace
    {
        auto in_sphere(glm::vec3 pos, glm::vec3 center, float radius) -> bool
        {
            auto delta = pos - center;
            return (dot(delta, delta) <= radius*radius);
        }

        auto front_facing(swing::face_type const* f, math::direction_type front) -> bool
        {
            auto n = swing::normal(f->boundary->any_hege);
            return (dot(*n, *front) > 1e-5f);
        }

        auto match_no_backfaces(swing::face_type const* f, float3 front3, bool no_backfaces) -> bool
        {
            if (no_backfaces) {
                auto front = math::direction_type{to_glm(front3)};
                if (!front_facing(f, front)) return false;
            }
            return true;
        }

        auto match(swing::vert_type const* v, op_fields_selection_select_verts const& fields) -> bool
        {
            return in_sphere(v->pos, to_glm(fields.center), fields.radius);
        }

        auto match(swing::face_type const* f, op_fields_selection_select_faces const& fields) -> bool
        {
            glm::vec3 median{};
            {
                int vert_count{};
                for (auto& h: swing::list::iterate(f->boundary->any_hege)) {
                    median += h.start->pos;
                    vert_count++;
                }
                median /= float(vert_count);
            }

            if (!in_sphere(median, to_glm(fields.center), fields.radius))
                return false;

            if (!match_no_backfaces(f, fields.front, fields.no_backfaces))
                return false;

            return true;
        }
    }

    auto invoke(op_fields_selection_select_verts const& fields, util::span<lib::any> args) -> lib::any
    {
        auto m = extract_or_croak<model>(args[0], "Argument must be a model.");

        auto& slab = m.hmesh.verts;
        std::unordered_set<swing::vert_type*> frees{begin(slab.frees), end(slab.frees)};

        std::vector<swing::vert_type*> verts;
        if (fields.radius >= 0.0f) {
            for (auto& node: slab.nodes)
                if (frees.count(&node) == 0 && match(&node, fields))
                    verts.emplace_back(&node);
        }
        select_verts(m, fields.exclusive, verts, fields.inverse);

        return std::move(m);
    }

    auto invoke(op_fields_selection_select_faces const& fields, util::span<lib::any> args) -> lib::any
    {
        auto m = extract_or_croak<model>(args[0], "Argument must be a model.");

        auto& slab = m.hmesh.faces;
        std::unordered_set<swing::face_type*> frees{begin(slab.frees), end(slab.frees)};

        std::vector<swing::face_type*> faces;
        if (fields.radius >= 0.0f) {
            for (auto& node: slab.nodes)
                if (frees.count(&node) == 0 && match(&node, fields))
                    faces.emplace_back(&node);
        }
        select_faces(m, fields.exclusive, faces, fields.inverse);

        if (fields.affect_verts) {
            std::vector<swing::vert_type*> verts;
            for (auto face: faces)
                for (auto& r: swing::list::iterate(face->boundary))
                    for (auto& h: swing::list::iterate(r.any_hege))
                        verts.emplace_back(h.start);
            select_verts(m, fields.exclusive, verts, fields.inverse);
        }

        return std::move(m);
    }

    auto invoke(op_fields_selection_containing_faces const& fields, util::span<lib::any> args) -> lib::any
    {
        auto m = extract_or_croak<model>(args[0], "Argument must be a model.");

        std::vector<swing::face_type*> faces;
        for (auto vert: m.vert_selection) {
            for (auto& h: swing::iter::heges_around_vert(vert->any_hege)) {
                auto face = h.ring->face;
                if (fields.only_boundary && face->boundary != h.ring)
                    continue;
                if (!match_no_backfaces(face, fields.front, fields.no_backfaces))
                    continue;
                faces.emplace_back(face);
            }
        }
        select_faces(m, fields.exclusive, faces, fields.inverse);

        if (fields.affect_verts) {
            std::vector<swing::vert_type*> verts;
            for (auto face: faces) {
                if (fields.only_boundary) {
                    for (auto& h: swing::list::iterate(face->boundary->any_hege))
                        verts.emplace_back(h.start);
                } else {
                    for (auto& r: swing::list::iterate(face->boundary))
                        for (auto& h: swing::list::iterate(r.any_hege))
                            verts.emplace_back(h.start);
                }
            }
            select_verts(m, fields.exclusive, verts, fields.inverse);
        }

        return std::move(m);
    }
}

