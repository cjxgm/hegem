#include "../../lib/std/any.hxx"
#include "../../tool/span.hxx"
#include "../../kul/timeline.hxx"
#include "../op.hxx"
#include "tool.hxx"
#include <utility>      // for std::move
#include <string>

namespace hegem::skein::op::invoke_impl
{
    auto invoke(op_fields_timeline_primitive_constant const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl = kul::timeline{};
        tl.evaluator = [x=fields.value] (float) -> float { return x; };
        tl.expression.partial_expression = std::to_string(fields.value);
        tl.value_min = fields.value;
        tl.value_max = fields.value;
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_primitive_ramp const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl = kul::timeline{};
        tl.evaluator = [] (float t) -> float { return t; };
        tl.expression.independent_variables_positions = {0u};
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_primitive_id const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl = kul::timeline{};
        tl.evaluator = [] (float) -> float { return 0.0f; };
        tl.expression.partial_expression = "kul_ID";
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_primitive_apply const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument #1 must be a timeline.");
        auto tl1 = extract_or_croak<kul::timeline>(args[1], "Argument #2 must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, f1=tl1.evaluator] (float t) -> float { return f0(f1(t)); };
        tl.expression = tl0.expression.apply(tl1.expression);
        tl.value_min = fields.value_min;
        tl.value_max = fields.value_max;
        tl.window_min = fields.window_min;
        tl.window_max = fields.window_max;
        return std::move(tl);
    }
}

