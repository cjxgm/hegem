#include "../../lib/std/any.hxx"
#include "../../tool/span.hxx"
#include "../../swing/op/sweep.hxx"
#include "../../swing/op/euler.hxx"
#include "../../swing/geometry.hxx"
#include "../../swing/list.hxx"
#include "../../math/direction.hxx"
#include "../op.hxx"
#include "model.hxx"
#include "select.hxx"
#include "tool.hxx"
#include <utility>      // for std::move
#include <stdexcept>
#include <vector>

namespace hegem::skein::op::invoke_impl
{
    namespace
    {
        auto is_double_sided(swing::face_type* f) -> bool
        {
            auto h = f->boundary->any_hege;
            auto f2 = h->twin->ring->face;

            if (f == f2)
                return false;       // single-sided

            for (auto& h: swing::list::iterate(f->boundary->any_hege))
                if (h.twin->ring->face != f2)
                    return false;   // more than 2 faces or more vertices than opposite face

            for (auto& h2: swing::list::iterate(f2->boundary->any_hege))
                if (h2.twin->ring->face != f)
                    return false;   // more than 2 faces or more vertices than opposite face

            return true;
        }

        auto is_simple_double_sided(swing::face_type* f) -> bool
        {
            auto f2 = f->boundary->any_hege->twin->ring->face;

            if (f->boundary->next != f->boundary)
                return false;       // not simple

            if (f2->boundary->next != f2->boundary)
                return false;       // opposite face not simple

            return is_double_sided(f);
        }

        auto is_simple_double_sided(swing::body_type* b) -> bool
        {
            if (b->any_face == nullptr)
                return false;       // no faces

            if (b->any_face->next->next != b->any_face)
                return false;       // more than 2 faces

            return is_simple_double_sided(b->any_face);
        }

        auto is_simple_double_sided(swing::hemesh& m) -> bool
        {
            for (auto& b: swing::list::iterate(m.any_body))
                if (!is_simple_double_sided(&b))
                    return false;
            return true;
        }

        auto project(glm::vec3 pos, math::direction_type normal, glm::vec3 plain_point) -> glm::vec3
        {
            return pos - normal * dot(pos - plain_point, *normal);
        }
    }

    auto invoke(op_fields_advanced_merge const& fields, tool::span<lib::any> args) -> lib::any
    {
        model m;
        for (auto& arg: args.range()) {
            auto m2 = extract_or_croak<model>(arg, "Arguments must be models.");
            m.hmesh.extend(m2.hmesh);
        }

        select_all(m);

        return m;
    }

    auto invoke(op_fields_advanced_extrude const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto m = extract_or_croak<model>(args[0], "Argument must be a model.");

        std::vector<swing::vert_type*> verts;

        for (auto face: m.face_selection) {
            auto n = swing::normal(face->boundary->any_hege);
            try {
                swing::extrude(m.hmesh, face, n * fields.amount);
            }
            catch (std::invalid_argument const& e) {
                throw std::runtime_error{e.what()};
            }

            if (fields.select_verts) {
                for (auto& r: swing::list::iterate(face->boundary))
                    for (auto& h: swing::list::iterate(r.any_hege))
                        verts.emplace_back(h.start);
            }
        }

        if (fields.select_verts)
            select_verts(m, true, verts, false);

        return std::move(m);
    }

    auto invoke(op_fields_advanced_embed const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto target = extract_or_croak<model>(args[0], "First argument must be a model.");
        auto source = extract_or_croak<model>(args[1], "Second argument must be a model.");

        if (target.face_selection.size() != 1)
            throw std::runtime_error{"First argument must select 1 single face"};

        auto front_face = *target.face_selection.begin();
        auto front_hege = front_face->boundary->any_hege;

        if (front_hege == nullptr)
            throw std::runtime_error{"The selected face from first argument must have an edge."};

        if (!is_simple_double_sided(source.hmesh)) {
            throw std::runtime_error{
                "Second argument must only contain double-sided faces without inner rings."
            };
        }

        auto& m = target.hmesh;
        if (m.any_body == nullptr) return std::move(target);

        auto back = m.any_body->prev;
        m.extend(source.hmesh);
        source = {};

        // Detach source bodies
        std::vector<swing::body_type*> bodys;
        for (auto& b: swing::list::iterate(m.any_body, back->next))
            bodys.emplace_back(&b);
        swing::list::connect(back, m.any_body);

        auto front_normal = swing::normal(front_hege);
        auto front_pos = front_hege->start->pos;

        auto punch_hole = (!fields.no_holes && is_double_sided(front_face));
        decltype(target.face_selection) faces;

        for (auto b: bodys) {
            auto facing_hege = b->any_face->boundary->any_hege;
            if (facing_hege == nullptr) {
                m.free(b->any_face->boundary);
                m.free(b->any_face);
                continue;
            }

            auto counter_hege = facing_hege->twin;
            {
                auto n = swing::normal(facing_hege);
                if (dot(*front_normal, *n) < 0.0f)
                    std::swap(facing_hege, counter_hege);
            }

            auto  facing_face =  facing_hege->ring->face;
            auto counter_face = counter_hege->ring->face;

            for (auto& h: swing::list::iterate(facing_hege)) {
                auto& p = h.start->pos;
                p = project(p, front_normal, front_pos);
            }

            swing::face_to_ring(m, counter_face, front_face);

            if (punch_hole) {
                auto back_face = front_hege->twin->ring->face;
                swing::face_to_ring(m, facing_face, back_face);
            } else {
                swing::list::insert_after(front_face, facing_face);
                facing_face->body = front_face->body;
                faces.emplace(facing_face);
            }
        }

        for (auto b: bodys) m.free(b);
        bodys = {};

        if (!punch_hole) target.face_selection = std::move(faces);

        return std::move(target);
    }
}

