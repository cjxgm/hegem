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

    template <auto* Drop, typename Declaration>
    struct wild_canonical_drop;

    template <auto* Drop, typename Item, typename Result>
    struct wild_canonical_drop<Drop, auto (*) (Item item) -> Result>
    {
        static constexpr auto drop(Item maybe_item) -> void
        {
            if (auto& item = maybe_item) {
                (void) Drop(item);
            } else {
                // Silently do nothing.
            }
        }
    };

    template <typename Item, typename Dropping_Item>
    inline static auto defer(auto (*drop) (Dropping_Item item) -> void, Item item) -> auto
    {
        return guard{[drop, item] { drop(item); }};
    }

    template <auto* Drop>
    inline constexpr auto* canonical_drop = &wild_canonical_drop<Drop, decltype(Drop)>::drop;
}

