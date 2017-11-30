#include "../../lib/std/any.hh"
#include "../../util/span.hh"
#include "../op.hh"
#include <utility>      // for std::move

namespace rt::sk::op::invoke_impl
{
    auto invoke(op_fields_primitive_poly_disk const& fields, util::span<lib::any> args) -> lib::any
    {
        return {};
    }

    auto invoke(op_fields_primitive_cube const& fields, util::span<lib::any> args) -> lib::any
    {
        return {};
    }
}

