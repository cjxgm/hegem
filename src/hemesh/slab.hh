#pragma once
#include <type_traits>
#include <vector>
#include <deque>

namespace rt::hemesh
{
    // For your "Rule of Zero" needs.
    //
    // The allocated object is guranteed to be value initialized
    // (default ctor is called, zeroed otherwise).
    template <class T>
    struct unbound_slab
    {
        using value_type = T;

        static_assert(std::is_trivially_copyable_v<value_type>,
            "Unbound arena only works for trivially copyable types.");

        auto* alloc()
        {
            if (frees.empty())
                return &nodes.emplace_back(value_type{});

            auto* x = frees.back();
            frees.pop_back();
            *x = value_type{};
            return x;
        }

        void free(value_type* x)
        {
            frees.emplace_back(x);
        }

        std::deque<value_type> nodes;
        std::vector<value_type*> frees;
    };
}

