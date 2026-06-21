#include "../../lib/std/any.hxx"
#include "../../math/modulus.hxx"
#include "../../tool/span.hxx"
#include "../../kul/timeline.hxx"
#include "../op.hxx"
#include "tool.hxx"
#include <utility>      // for std::move, std::swap
#include <string>
#include <cmath>

namespace hegem::skein::op::invoke_impl
{
    inline namespace
    {
        auto kul_mod(float a, float b) noexcept -> float
        {
            if (b == 0.0f) return 0.0f;
            return math::modulus(a, b);
        }
    }

    auto invoke(op_fields_timeline_timewarping_timeshift const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, amount=fields.amount] (float t) -> float { return f0(t - amount); };
        tl.expression.partial_expression = "( - " + std::to_string(fields.amount) + ")";
        tl.expression.independent_variables_positions = {1u};
        tl.expression = tl0.expression.apply(tl.expression);
        tl.value_min = tl0.value_min;
        tl.value_max = tl0.value_max;
        tl.window_min = tl0.window_min + fields.amount;
        tl.window_max = tl0.window_max + fields.amount;
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_timewarping_speedup const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, scale=fields.scale] (float t) -> float { return f0(t * scale); };
        tl.expression.partial_expression = "( * " + std::to_string(fields.scale) + ")";
        tl.expression.independent_variables_positions = {1u};
        tl.expression = tl0.expression.apply(tl.expression);
        tl.value_min = tl0.value_min;
        tl.value_max = tl0.value_max;
        tl.window_min = tl0.window_min * fields.scale;
        tl.window_max = tl0.window_max * fields.scale;
        if (tl.window_max < tl.window_min) std::swap(tl.window_min, tl.window_max);
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_timewarping_window const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, size=fields.size] (float t) -> float { return f0(kul_mod(t, size)); };
        tl.expression.partial_expression = "kul_mod(, " + std::to_string(fields.size) + ")";
        tl.expression.independent_variables_positions = {8u};
        tl.expression = tl0.expression.apply(tl.expression);
        tl.value_min = tl0.evaluator(0.0f);
        tl.value_max = tl0.evaluator(fields.size);
        if (tl.value_max < tl.value_min) std::swap(tl.value_min, tl.value_max);
        tl.window_min = 0.0f;
        tl.window_max = std::abs(fields.size);
        return std::move(tl);
    }
}

