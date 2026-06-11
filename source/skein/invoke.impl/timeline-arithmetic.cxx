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
    namespace
    {
        // Calculate f1(t) / f2(t).
        // When f2(t) == 0, returns approximation of lim_{x -> t} {f1(x) / f2(x)}
        // via L'hopital's Rule.
        template <class Fn1, class Fn2>
        auto kul_div(Fn1 f1, Fn2 f2, float t) noexcept -> float
        {
            auto v1 = f1(t);
            auto v2 = f2(t);
            if (v2 == 0.0f) {
                auto v1d = f1(t + 1e-5f);
                auto v2d = f2(t + 1e-5f);
                if (v2d == v2) {
                    return 0.0f;
                } else {
                    return (v1 - v1d) / (v2 - v2d);
                }
            } else {
                return v1 / v2;
            }
        }

        auto kul_mod(float a, float b) noexcept -> float
        {
            if (b == 0.0f) return 0.0f;
            return math::modulus(a, b);
        }
    }

    auto invoke(op_fields_timeline_arithmetic_add const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument #1 must be a timeline.");
        auto tl1 = extract_or_croak<kul::timeline>(args[1], "Argument #2 must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, f1=tl1.evaluator] (float t) -> float { return f0(t) + f1(t); };

        kul::independent_expression iexprs[2]{
            std::move(tl0.expression),
            std::move(tl1.expression),
        };
        tl.expression.partial_expression = "( + )";
        tl.expression.independent_variables_positions = {1u, 4u};
        tl.expression = tl.expression.apply(tool::span<kul::independent_expression const>{iexprs});

        tl.value_min = tl0.value_min + tl1.value_min;
        tl.value_max = tl0.value_max + tl1.value_max;
        tl.window_min = std::min(tl0.window_min, tl1.window_min);
        tl.window_max = std::max(tl0.window_max, tl1.window_max);
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_arithmetic_sub const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument #1 must be a timeline.");
        auto tl1 = extract_or_croak<kul::timeline>(args[1], "Argument #2 must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, f1=tl1.evaluator] (float t) -> float { return f0(t) - f1(t); };

        kul::independent_expression iexprs[2]{
            std::move(tl0.expression),
            std::move(tl1.expression),
        };
        tl.expression.partial_expression = "( - )";
        tl.expression.independent_variables_positions = {1u, 4u};
        tl.expression = tl.expression.apply(tool::span<kul::independent_expression const>{iexprs});

        tl.value_min = tl0.value_min - tl1.value_max;
        tl.value_max = tl0.value_max - tl1.value_min;
        tl.window_min = std::min(tl0.window_min, tl1.window_min);
        tl.window_max = std::max(tl0.window_max, tl1.window_max);
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_arithmetic_mul const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument #1 must be a timeline.");
        auto tl1 = extract_or_croak<kul::timeline>(args[1], "Argument #2 must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, f1=tl1.evaluator] (float t) -> float { return f0(t) * f1(t); };

        kul::independent_expression iexprs[2]{
            std::move(tl0.expression),
            std::move(tl1.expression),
        };
        tl.expression.partial_expression = "( * )";
        tl.expression.independent_variables_positions = {1u, 4u};
        tl.expression = tl.expression.apply(tool::span<kul::independent_expression const>{iexprs});

        tl.value_min = tl0.value_min * tl1.value_max;
        tl.value_max = tl0.value_max * tl1.value_min;
        if (tl.value_max < tl.value_min) std::swap(tl.value_min, tl.value_max);
        tl.window_min = std::min(tl0.window_min, tl1.window_min);
        tl.window_max = std::max(tl0.window_max, tl1.window_max);
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_arithmetic_div const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument #1 must be a timeline.");
        auto tl1 = extract_or_croak<kul::timeline>(args[1], "Argument #2 must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, f1=tl1.evaluator] (float t) -> float { return kul_div(f0, f1, t); };

        kul::independent_expression iexprs[2]{
            std::move(tl0.expression),
            std::move(tl1.expression),
        };
        tl.expression.partial_expression = "kul_div(, )";
        tl.expression.independent_variables_positions = {8u, 10u};
        tl.expression = tl.expression.apply(tool::span<kul::independent_expression const>{iexprs});

        tl.value_min = kul_div([&] (float) { return tl0.value_min; }, [&] (float) { return tl1.value_max; }, 0.0f);
        tl.value_max = kul_div([&] (float) { return tl0.value_max; }, [&] (float) { return tl1.value_min; }, 0.0f);
        if (tl.value_max < tl.value_min) std::swap(tl.value_min, tl.value_max);
        tl.window_min = std::min(tl0.window_min, tl1.window_min);
        tl.window_max = std::max(tl0.window_max, tl1.window_max);
        return std::move(tl);
    }

    auto invoke(op_fields_timeline_arithmetic_mod const& fields, tool::span<lib::any> args) -> lib::any
    {
        auto tl0 = extract_or_croak<kul::timeline>(args[0], "Argument #1 must be a timeline.");
        auto tl1 = extract_or_croak<kul::timeline>(args[1], "Argument #2 must be a timeline.");
        auto tl = kul::timeline{};
        tl.evaluator = [f0=tl0.evaluator, f1=tl1.evaluator] (float t) -> float { return kul_mod(f0(t), f1(t)); };

        kul::independent_expression iexprs[2]{
            std::move(tl0.expression),
            std::move(tl1.expression),
        };
        tl.expression.partial_expression = "kul_mod(, )";
        tl.expression.independent_variables_positions = {8u, 10u};
        tl.expression = tl.expression.apply(tool::span<kul::independent_expression const>{iexprs});

        tl.window_min = 0.0f;
        tl.window_max = std::abs(tl1.value_max);
        return std::move(tl);
    }
}

