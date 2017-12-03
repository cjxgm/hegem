#pragma once
// Operators for generic doubly-linked cyclic lists.
#include "../util/priter.hh"
#include <utility>      // for std::forward

namespace rt::hegem
{
    namespace list
    {
        template <class T>
        void connect(T* from, T* to)
        {
            if (from) from->next = to;
            if (to)     to->prev = from;
        }

        template <class T>
        void remove(T* x)
        {
            connect(x->prev, x->next);
        }

        template <class T>
        T* insert_after(T* pivot, T* x)
        {
            connect(x, pivot->next);
            connect(pivot, x);
            return x;
        }

        template <class T>
        T* insert_before(T* pivot, T* x)
        {
            connect(pivot->prev, x);
            connect(x, pivot);
            return x;
        }

        template <class T>
        T* append(T*& first, T* x)
        {
            if (!first) first = x;
            return insert_before(first, x);
        }

        template <class T>
        void extend(T*& first, T* x)
        {
            if (first) {
                auto a = first;
                auto b = first->prev;
                auto c = x;
                auto d = x->prev;
                connect(b, c);
                connect(d, a);
            } else {
                first = x;
            }
        }

        template <class Next_Policy>
        struct priter
        {
            using next_policy = Next_Policy;
            using value_type = typename next_policy::value_type;

            priter() = default;
            priter(value_type* first, value_type* start=nullptr)
                : now{start ? (start == first ? nullptr : start) : first}
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

