#include "integral-allocator.hxx"
#include <cassert>

namespace hegem::util
{
    auto integral_allocator::allocate() -> value_type
    {
        auto x = free;
        if (x == static_cast<value_type>(next.size()))
            next.emplace_back(x+1);
        assert(x < static_cast<value_type>(next.size()));

        free = next[x];
        return x;
    }

    void integral_allocator::deallocate(value_type x)
    {
        if (x == -1) return;
        assert(x >= 0);
        assert(x < static_cast<value_type>(next.size()));
        next[x] = free;
        free = x;
    }
}

