#pragma once
// A pointer and a size to represent an array
#include "range.hxx"
#include <vector>

namespace hegem::tool
{
    template <class T>
    struct span
    {
        using value_type = T;

        span(): span{nullptr, 0} {}
        span(value_type* ptr, int size): ptr_{ptr}, size_{size} {}

        template <int N>
        span(value_type (&arr)[N]): span{arr, N} {}

        template <class U>
        span(std::vector<U> & x): span{x.data(), int(x.size())} {}

        auto base() const { return ptr_; }
        auto size() const { return size_; }
        auto range() const { return tool::range{ptr_, ptr_+size_}; }
        auto range(int i) const { return tool::range{ptr_+i, ptr_+size_}; }

        auto& operator [] (int i) const { return ptr_[i]; }

    private:
        value_type* ptr_;
        int size_;
    };

    template <class T>
    struct const_span
    {
        using value_type = T;

        const_span(): const_span{nullptr, 0} {}
        const_span(value_type const* ptr, int size): ptr_{ptr}, size_{size} {}

        template <int N>
        const_span(value_type const (&arr)[N]): const_span{arr, N} {}

        template <class U>
        const_span(std::vector<U> const& x): const_span{x.data(), int(x.size())} {}

        auto base() const { return ptr_; }
        auto size() const { return size_; }
        auto range() const { return tool::range{ptr_, ptr_+size_}; }
        auto range(int i) const { return tool::range{ptr_+i, ptr_+size_}; }

        auto& operator [] (int i) const { return ptr_[i]; }

    private:
        value_type const* ptr_;
        int size_;
    };
}

