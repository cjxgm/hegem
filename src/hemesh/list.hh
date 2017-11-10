#pragma once
// Operators for generic doubly-linked cyclic lists.
#include "../util/priter.hh"
#include <utility>      // for std::forward

namespace rt::hemesh
{
    namespace list
    {
        template <class T>
        T* insert_after(T* pivot, T* x)
        {
            x->next = pivot->next;
            x->prev = pivot;
            x->prev->next = x;
            x->next->prev = x;
            return x;
        }

        template <class T>
        T* insert_before(T* pivot, T* x)
        {
            x->next = pivot;
            x->prev = pivot->prev;
            x->next->prev = x;
            x->prev->next = x;
            return x;
        }

        template <class T>
        T* append(T*& a, T* x)
        {
            if (!a) a = x;
            return insert_before(a, x);
        }

        template <class Next_Policy>
        struct priter
        {
            using next_policy = Next_Policy;
            using value_type = typename next_policy::value_type;

            priter() = default;
            priter(value_type* first, value_type* start=nullptr)
                : now{start ? start : first}
                , last{first}   // In doubly-linked cyclic lists, the first is also the last.
            {}

            value_type* operator ++ ()
            {
                if (now == nullptr)
                    return nullptr;

                auto result = now;
                now = next(now);
                if (now == last)
                    now = nullptr;
                return result;
            }

            static value_type* next(value_type* p) { return next_policy::next(p); }

        private:
            value_type* now;
            value_type* last;
        };

        template <class Next_Policy, class ...Ts>
        auto range(Ts &&... xs)
        {
            return util::range_from_priter(
                priter<Next_Policy>{
                    std::forward<Ts>(xs)...,
                });
        }

        template <class T>
        auto iterate(T* first, T* start=nullptr)
        {
            struct next_node
            {
                using value_type = T;
                static value_type* next(value_type* p) { return p->next; }
            };

            return range<next_node>(first, start);
        }

        template <class T>
        auto reverse_iterate(T* first, T* start=nullptr)
        {
            struct prev_node
            {
                using value_type = T;
                static value_type* next(value_type* p) { return p->prev; }
            };

            return range<prev_node>(first, start);
        }
    }
}

