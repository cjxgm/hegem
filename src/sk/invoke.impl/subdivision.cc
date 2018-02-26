#include "../../lib/std/any.hh"
#include "../../util/span.hh"
#include "../../hegem/op/subsurf.hh"
#include "../op.hh"
#include "model.hh"
#include "util.hh"
#include "select.hh"
#include <utility>      // for std::move

namespace rt::sk::op::invoke_impl
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
        while (level--)
            subdivide_catmull_clark(m);

        select_none(md);
        return std::move(md);
    }
}

