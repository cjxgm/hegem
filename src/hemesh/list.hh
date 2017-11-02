#pragma once

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
    }
}

