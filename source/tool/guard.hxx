#pragma once
#include <utility>  // for std::forward

namespace hegem::tool
{
    template <typename Drop>
    struct guard
    {
        Drop drop;

        ~guard() { drop(); }

        template <typename Argument>
        guard(Argument&& drop): drop{std::forward<Argument>(drop)} {}
    };

    template <typename Drop>
    guard(Drop drop) -> guard<Drop>;

    template <typename Item, typename Dropping_Item>
    inline static auto defer(auto (*drop) (Dropping_Item item) -> void, Item item) -> auto
    {
        return guard{[drop, item] { drop(item); }};
    }
}

