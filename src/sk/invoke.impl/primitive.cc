#include "../../lib/std/any.hh"
#include "../../util/span.hh"
#include "../../hegem/op/shape.hh"
#include "../op.hh"
#include "model.hh"
#include <utility>      // for std::move

namespace rt::sk::op::invoke_impl
{
    auto invoke(op_fields_primitive_poly_disk const& fields, util::span<lib::any> args) -> lib::any
    {
        model m;
        auto radius = (fields.radius < 1e-3f ? 1e-3f : fields.radius);
        auto ngon = (fields.ngon < 3 ? 3 : fields.ngon);
        hegem::make_polygon_disk(m.hmesh, ngon, radius);
        return m;
    }

    auto invoke(op_fields_primitive_cube const& fields, util::span<lib::any> args) -> lib::any
    {
        model m;
        hegem::make_cube(m.hmesh);
        return m;
    }
}

