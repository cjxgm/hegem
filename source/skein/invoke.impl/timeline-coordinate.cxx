#include "../../lib/std/any.hxx"
#include "../../util/span.hxx"
#include "../../kul/timeline.hxx"
#include "../op.hxx"
#include "util.hxx"
#include <utility>      // for std::move
#include <string>
#include <cmath>

namespace hegem::skein::op::invoke_impl
{
    auto invoke(op_fields_timeline_coordinate_polar_to_cartesian_x const& fields, util::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument #1 must be a timeline.");
        auto tl1 = extract_or_croak<kul::timeline>(args[1], "Argument #2 must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, f1=tl1.evaluator] (float t) -> float { return std::cos(f0(t)) * f1(t); };

        kul::independent_expression iexprs[2]{
            std::move(tl0.expression),
            std::move(tl1.expression),
        };
        tl.expression.partial_expression = "(kul_cos() * )";
        tl.expression.independent_variables_positions = {9u, 13u};
        tl.expression = tl.expression.apply(util::span<kul::independent_expression const>{iexprs});

        tl.value_min = 0.0f;
        tl.value_max = std::max(std::abs(tl1.value_min), std::abs(tl1.value_max));
        tl.window_min = std::min(tl0.window_min, tl1.window_min);
        tl.window_max = std::max(tl0.window_max, tl1.window_max);
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_coordinate_polar_to_cartesian_y const& fields, util::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument #1 must be a timeline.");
        auto tl1 = extract_or_croak<kul::timeline>(args[1], "Argument #2 must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, f1=tl1.evaluator] (float t) -> float { return std::sin(f0(t)) * f1(t); };

        kul::independent_expression iexprs[2]{
            std::move(tl0.expression),
            std::move(tl1.expression),
        };
        tl.expression.partial_expression = "(kul_sin() * )";
        tl.expression.independent_variables_positions = {9u, 13u};
        tl.expression = tl.expression.apply(util::span<kul::independent_expression const>{iexprs});

        tl.value_min = 0.0f;
        tl.value_max = std::max(std::abs(tl1.value_min), std::abs(tl1.value_max));
        tl.window_min = std::min(tl0.window_min, tl1.window_min);
        tl.window_max = std::max(tl0.window_max, tl1.window_max);
        return std::move(tl);
    }
}

