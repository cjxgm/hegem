#include "timeline.hh"
#include <utility>      // for std::move

namespace rt::kul
{
    namespace
    {
        auto zero_expr() noexcept -> independent_expression
        {
            auto result = independent_expression{};
            result.partial_expression = "0";
            return result;
        }
    }

    auto independent_expression::apply(std::string x) const noexcept -> std::string
    {
        auto xx = independent_expression{};
        xx.partial_expression = std::move(x);

        auto result = apply(xx);

        return std::move(result.partial_expression);
    }

    auto independent_expression::apply(independent_expression const& x) const noexcept -> independent_expression
    {
        return apply(util::span<independent_expression const>{&x, 1});
    }

    auto independent_expression::apply(util::span<independent_expression const> xs) const noexcept -> independent_expression
        // Assumes: independent_variables_positions is monotonically increasing.
    {
        auto offset = std::size_t{};
        auto const src_expr = zero_expr();
        auto src_iv = &src_expr;
        auto result = independent_expression{};
        result.partial_expression = partial_expression;

        auto x = xs.range().begin();
        auto xlast = xs.range().end();
        for (auto iv_pos: independent_variables_positions) {
            if (x < xlast) src_iv = &*x++;

            result.partial_expression.insert(iv_pos + offset, src_iv->partial_expression);

            for (auto src_pos: src_iv->independent_variables_positions)
                result.independent_variables_positions.emplace_back(iv_pos + offset + src_pos);

            offset += src_iv->partial_expression.size();
        }

        return result;
    }

    auto timeline::printable_expression() noexcept -> std::string
    {
        return expression.apply("@");
    }
}

