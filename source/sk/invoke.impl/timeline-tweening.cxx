#include "../../lib/std/any.hxx"
#include "../../util/span.hxx"
#include "../../kul/timeline.hxx"
#include "../op.hxx"
#include "util.hxx"
#include <utility>      // for std::move
#include <string>
#include <cmath>

namespace hegem::sk::op::invoke_impl
{
    namespace
    {
        auto easeout(float t, float target, float steepness) noexcept -> float
        {
            if (t <= 0.0f) return 0.0f;
            return target * (1.0f - std::pow(std::pow(2.0f, steepness) + 1.0f, -t));
        }
    }

    auto invoke(op_fields_timeline_tweening_easeout const& fields, util::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, target=fields.target, steepness=fields.steepness] (float t) -> float { return easeout(f0(t), target, steepness); };
        tl.expression.partial_expression = "kul_easeout(, " + std::to_string(fields.target) + ", " + std::to_string(fields.steepness) + ")";
        tl.expression.independent_variables_positions = {12u};
        tl.expression = tl.expression.apply(tl0.expression);
        tl.value_min = 0.0f;
        tl.value_max = fields.target;
        tl.window_min = tl0.window_min;
        tl.window_max = tl0.window_max;
        return std::move(tl);
    }
}

