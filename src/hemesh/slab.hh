#pragma once
// A slab allocator is an object pool with a free list.
#include <type_traits>
#include <vector>
#include <deque>

namespace rt::hemesh
{
    // The one implemented here is special because it's designed for
    // trivially copyable types, hence there is "trivial" in its name.
    //
    // Unbound here means there is no fixed budget of the pool size.
    // The pool will increase its size dynamically.
    //
    // [Exception Safety]: Guranteed
    template <class T>
    struct unbound_trivial_slab
    {
        using value_type = T;

        static_assert(std::is_trivially_copyable_v<value_type>,
            "unbound_trivial_slab<T> only works when T is trivially copyable.");

        // Gurantees:
        // - The allocated value is value-initialized (zeroed out).
        // - The pointer has the same lifetime as the slab itself.
        value_type* alloc()
        {
            if (frees.empty())
                return &nodes.emplace_back(value_type{});

            auto* x = frees.back();
            frees.pop_back();
            return x;
        }

        // Guranteed Outcome:
        // - The freed value is value-initialized (zeroed out)
        //   after successfully put into the free list.
        void free(value_type* x)
        {
            frees.emplace_back(x);
            *x = value_type{};
        }

        int capacity() const noexcept { return int(nodes.size()); }
        int size() const noexcept { return capacity() - int(frees.size()); }

        // Invariants:
        // - frees.size() <= nodes.size()
        // - resizing nodes does not invalidate pointers to any node.
        std::deque<value_type> nodes;
        std::vector<value_type*> frees;
    };
}

