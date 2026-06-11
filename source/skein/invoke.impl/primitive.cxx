#include "../../lib/std/any.hxx"
#include "../../util/span.hxx"
#include "../../swing/op/shape.hxx"
#include "../op.hxx"
#include "model.hxx"
#include "select.hxx"
#include <utility>      // for std::move

namespace hegem::skein::op::invoke_impl
{
    auto invoke(op_fields_primitive_poly_disk const& fields, util::span<lib::any> args) -> lib::any
    {
        model m;
        auto radius = (fields.radius < 1e-3f ? 1e-3f : fields.radius);
        auto ngon = (fields.ngon < 3 ? 3 : fields.ngon);
        swing::make_polygon_disk(m.hmesh, ngon, radius);
        select_all(m);
        return std::move(m);
    }

    auto invoke(op_fields_primitive_cube const& fields, util::span<lib::any> args) -> lib::any
    {
        model m;
        swing::make_cube(m.hmesh);
        select_all(m);
        return std::move(m);
    }
}

