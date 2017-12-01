#include "../../lib/glm/op/geom.hh"
#include "../../lib/std/any.hh"
#include "../../util/span.hh"
#include "../../math/direction.hh"
#include "../../hegem/list.hh"
#include "../../hegem/geometry.hh"
#include "../op.hh"
#include "model.hh"
#include "select.hh"
#include "util.hh"
#include <utility>      // for std::move

namespace rt::sk::op::invoke_impl
{
    namespace
    {
        auto in_sphere(glm::vec3 pos, glm::vec3 center, float radius) -> bool
        {
            auto delta = pos - center;
            return (dot(delta, delta) <= radius*radius);
        }

        auto match(hegem::vert_type const* v, op_fields_selection_select_verts const& fields) -> bool
        {
            return in_sphere(v->pos, to_glm(fields.center), fields.radius);
        }

        auto match(hegem::face_type const* f, op_fields_selection_select_faces const& fields) -> bool
        {
            glm::vec3 median{};
            {
                int vert_count{};
                for (auto& h: hegem::list::iterate(f->boundary->any_hege)) {
                    median += h.start->pos;
                    vert_count++;
                }
                median /= float(vert_count);
            }

            if (!in_sphere(median, to_glm(fields.center), fields.radius))
                return false;

            if (fields.no_backfaces) {
                auto n = hegem::normal(f->boundary->any_hege);
                auto front = math::direction_type{to_glm(fields.front)};
                if (!hegem::is_same_side(n, front))
                    return false;
            }

            return true;
        }
    }

    auto invoke(op_fields_selection_select_verts const& fields, util::span<lib::any> args) -> lib::any
    {
        auto m = extract_or_croak<model>(args[0], "Argument must be a model.");

        auto& slab = m.hmesh.verts;
        std::unordered_set<hegem::vert_type*> frees{begin(slab.frees), end(slab.frees)};

        std::vector<hegem::vert_type*> verts;
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
        std::unordered_set<hegem::face_type*> frees{begin(slab.frees), end(slab.frees)};

        std::vector<hegem::face_type*> faces;
        if (fields.radius >= 0.0f) {
            for (auto& node: slab.nodes)
                if (frees.count(&node) == 0 && match(&node, fields))
                    faces.emplace_back(&node);
        }
        select_faces(m, fields.exclusive, faces, fields.inverse);

        if (fields.affect_verts) {
            std::vector<hegem::vert_type*> verts;
            if (fields.radius >= 0.0f) {
                for (auto face: faces)
                    for (auto& r: hegem::list::iterate(face->boundary))
                        for (auto& h: hegem::list::iterate(r.any_hege))
                            verts.emplace_back(h.start);
            }
            select_verts(m, fields.exclusive, verts, fields.inverse);
        }

        return std::move(m);
    }
}

