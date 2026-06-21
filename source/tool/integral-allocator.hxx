#pragma once
#include <vector>
#include <limits>

namespace hegem::tool
{
    // Allocates integers from 0 to maximum value.
    struct integral_allocator
    {
        using value_type = int;

        value_type allocate();
        auto deallocate(value_type x) -> void;

    private:
        std::vector<value_type> next{};
        value_type free{};
    };
}

