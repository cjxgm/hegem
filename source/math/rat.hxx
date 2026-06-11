#pragma once
// very primitive rational number
#include <type_traits>
#include <stdexcept>

namespace hegem::math
{
    template <class T>
    struct rat
    {
        using value_type = T;

        static_assert(std::is_integral_v<value_type>);

        rat(): rat{zero(), one()} {}
        rat(value_type x): rat{x, one()} {}

        // denominator is ensured to be positive
        rat(value_type num, value_type den)
            : num{den < 0 ? -num : num}
            , den{den < 0 ? -den : den}
        {
            if (den == 0)
                throw std::logic_error{"Denominator cannot be zero"};
        }

        explicit operator value_type () const { return num / den; }

        auto reciprocal() const { return rat{den, num}; }

        #define DEFINE_COMPARISON_OP(OP) \
            friend auto operator OP (rat<T> a, rat<T> b) -> bool \
            { \
                return (a.num * b.den OP b.num * a.den); \
            }
        DEFINE_COMPARISON_OP(<);
        DEFINE_COMPARISON_OP(>);
        DEFINE_COMPARISON_OP(==);
        #undef DEFINE_COMPARISON_OP

        #define DEFINE_ADDITIVE_OP(OP) \
            auto& operator OP##= (rat<T> a) \
            { \
                if (den % a.den == 0) { \
                    num += a.num * (den / a.den); \
                } else { \
                    num = num * a.den + a.num * den; \
                    den *= a.den; \
                } \
                return *this; \
            } \
            friend auto operator OP (rat<T> a, rat<T> b) -> rat<T> \
            { \
                return (a += b); \
            }
        DEFINE_ADDITIVE_OP(+);
        DEFINE_ADDITIVE_OP(-);
        #undef DEFINE_ADDITIVE_OP

    private:
        value_type num;
        value_type den;

        static constexpr value_type one() { return value_type(1); }
        static constexpr value_type zero() { return value_type(0); }
    };
}

