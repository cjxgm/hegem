#include "../../lib/std/any.hxx"
#include "../../util/span.hxx"
#include "../../swing/op/subsurf.hxx"
#include "../op.hxx"
#include "model.hxx"
#include "util.hxx"
#include "select.hxx"
#include <utility>      // for std::move

namespace hegem::skein::op::invoke_impl
{
    auto invoke(op_fields_subdivision_catmull_clark const& fields, util::span<lib::any> args) -> lib::any
    {
        auto md = extract_or_croak<model>(args[0], "Argument must be a model.");
        auto& m = md.hmesh;

        if (fields.triangulate)
            m = triangulate(std::move(m));

        auto level = fields.level;
        level =
            level < 0 ? 0 :
            level > 4 ? 4 :
            level;
        try {
            while (level--)
                subdivide_catmull_clark(m);
        }
        catch (std::invalid_argument const& e) {
            throw std::runtime_error{e.what()};
        }

        select_none(md);
        return std::move(md);
    }
}

