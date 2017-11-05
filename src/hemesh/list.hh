#pragma once
// Operators for generic doubly-linked cyclic lists.
#include "../util/priter.hh"

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

        template <class T>
        struct priter
        {
            using value_type = T;

            priter() = default;
            priter(value_type* first) : first{first}, now{first} {}

            value_type* operator ++ ()
            {
                if (now == nullptr)
                    return nullptr;

                auto result = now;
                now = now->next;
                if (now == first)
                    now = nullptr;
                return result;
            }

        private:
            value_type* first;
            value_type* now;
        };

        template <class T>
        auto iterate(T* first)
        {
            return util::range_from_priter(priter{first});
        }
    }
}

