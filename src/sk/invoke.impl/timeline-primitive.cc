#include "../../lib/std/any.hh"
#include "../../util/span.hh"
#include "../../kul/timeline.hh"
#include "../op.hh"
#include <utility>      // for std::move
#include <string>

namespace rt::sk::op::invoke_impl
{
    auto invoke(op_fields_timeline_primitive_constant const& fields, util::span<lib::any> args) -> lib::any
    {
        auto tl = kul::timeline{};
        tl.evaluator = [x=fields.value] (float) -> float { return x; };
        tl.expression.partial_expression = std::to_string(fields.value);
        tl.value_min = fields.value;
        tl.value_max = fields.value;
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_primitive_ramp const& fields, util::span<lib::any> args) -> lib::any
    {
        auto tl = kul::timeline{};
        tl.evaluator = [] (float t) -> float { return t; };
        tl.expression.independent_variables_positions = {0u};
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_primitive_id const& fields, util::span<lib::any> args) -> lib::any
    {
        auto tl = kul::timeline{};
        tl.evaluator = [] (float) -> float { return 0.0f; };
        tl.expression.partial_expression = "kul_ID";
        return std::move(tl);
    }
}

