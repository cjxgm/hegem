#pragma once
#include "../math/constants.hh"
#include "../util/span.hh"
#include <string>
#include <vector>
#include <functional>
#include <cstddef>

namespace rt::kul
{
    using timeline_evaluator_fn = auto (float time) -> float;
    using timeline_evaluator = std::function<timeline_evaluator_fn>;

    // Invariant:
    // - independent_variables_positions is monotonically increasing.
    struct independent_expression final
    {
        std::string partial_expression;
        std::vector<std::size_t> independent_variables_positions{};

        auto apply(std::string x) const noexcept -> std::string;
        auto apply(independent_expression const& x) const noexcept -> independent_expression;
        auto apply(util::span<independent_expression const> xs) const noexcept -> independent_expression;
    };

    struct timeline final
    {
        timeline_evaluator evaluator;
        independent_expression expression;
        float window_min{-math::inf};
        float window_max{+math::inf};
        float value_min{-math::inf};
        float value_max{+math::inf};

        auto printable_expression() noexcept -> std::string;
    };
}

