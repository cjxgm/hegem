#pragma once
// priter: PRe-Increament iTEratoR
//
// This module provides adapters to convert priters to/from iterators and ranges.
//
// A priter is always an input iterator (single-pass only). It has a structure of:
//
//      struct some_priter
//      {
//          some_priter() = default;
//
//          value_type* operator ++ ()
//          {
//              return {};
//          }
//      };
//
// A priter can be used like this:
//
//      auto x_pri = each_x();
//      while (auto x = ++x_pri) {
//          // use *x
//      }
#include "range.hh"
#include <utility>      // for std::move
#include <type_traits>

namespace rt::util
{
    template <class Iterator>
    struct priter_from_iterators
    {
        using iterator_type = Iterator;
        using value_type = typename iterator_type::value_type;

        priter_from_iterators() = default;
        priter_from_iterators(iterator_type first, iterator_type last)
            : next{first}
            , last{last}
        {}

        template <class Range>
        priter_from_iterators(Range const& range)
            : priter_from_iterators{begin(range), end(range)}
        {}

        value_type* operator ++ ()
        {
            if (next == last) return nullptr;
            auto* result = &*next;
            ++next;
            return result;
        }

    private:
        iterator_type next;
        iterator_type last;
    };

    template <class Priter>
    struct iterator_from_priter
    {
        using priter_type = Priter;
        using value_type = std::remove_pointer_t<decltype(++priter_type{})>;

        iterator_from_priter() = default;
        iterator_from_priter(priter_type priter)
            : priter{std::move(priter)}
            , value{++this->priter}
        {}

        value_type& operator * () const { return *value; }

        iterator_from_priter& operator ++ ()
        {
            value = ++priter;
            return *this;
        }

        friend bool operator != (iterator_from_priter const& a, iterator_from_priter const& b)
        {
            return (a.value != b.value);
        }

    private:
        priter_type priter;
        value_type* value{};
    };

    template <class Priter>
    auto range_from_priter(Priter p)
    {
        return range{
            iterator_from_priter<Priter>{std::move(p)},
            iterator_from_priter<Priter>{},
        };
    }
}

