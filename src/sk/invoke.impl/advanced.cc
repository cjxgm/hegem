#include "../../lib/std/any.hh"
#include "../../util/span.hh"
#include "../../hegem/op/sweep.hh"
#include "../../hegem/geometry.hh"
#include "../../hegem/list.hh"
#include "../op.hh"
#include "model.hh"
#include "select.hh"
#include "util.hh"
#include <utility>      // for std::move
#include <stdexcept>
#include <vector>

namespace rt::sk::op::invoke_impl
{
    auto invoke(op_fields_advanced_merge const& fields, util::span<lib::any> args) -> lib::any
    {
        model m;
        for (auto& arg: args.range()) {
            auto m2 = extract_or_croak<model>(arg, "Arguments must be models.");
            m.hmesh.extend(m2.hmesh);
        }

        select_all(m);

        return m;
    }

    auto invoke(op_fields_advanced_extrude const& fields, util::span<lib::any> args) -> lib::any
    {
        auto m = extract_or_croak<model>(args[0], "Argument must be a model.");

        if (fields.amount >= 1e-2f) {
            std::vector<hegem::vert_type*> verts;

            for (auto face: m.face_selection) {
                auto n = hegem::normal(face->boundary->any_hege);
                try {
                    hegem::extrude(m.hmesh, face, n * fields.amount);
                }
                catch (std::invalid_argument const& e) {
                    throw std::runtime_error(e.what());
                }

                if (fields.select_verts) {
                    for (auto& h: hegem::list::iterate(face->boundary->any_hege))
                        verts.emplace_back(h.start);
                }
            }

            if (fields.select_verts)
                select_verts(m, true, verts, false);
        }

        return std::move(m);
    }
}

