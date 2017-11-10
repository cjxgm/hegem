#pragma once
// Just a begin/end pair
#include <utility>      // for std::move

namespace rt::util
{
    template <class Iterator>
    struct range
    {
        using iterator = Iterator;

        range(iterator last): range{last, last} {}
        range(iterator first, iterator last)
            : first{std::move(first)}
            , last{std::move(last)}
        {}

        auto& begin() const { return first; }
        auto&   end() const { return last; }

    private:
        iterator first;
        iterator last;
    };
}

