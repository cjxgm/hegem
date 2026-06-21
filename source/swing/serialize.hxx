#pragma once
#include <string>
#include <utility>      // for std::forward
#include "hemesh.fwd.hxx"
#include "type.hxx"

namespace hegem::swing
{
    struct serializer
    {
        virtual auto declare_structure(char const* type, char const* name, void const* ptr) -> void = 0;

        virtual auto begin_structure(char const* type, char const* name, void const* ptr) -> void = 0;
        virtual auto end_structure() -> void = 0;
        virtual auto field_ptr_from_slab(char const* type, char const* name, void const* ptr) -> void = 0;
        virtual auto field(char const* type, char const* name, position_type pos) -> void = 0;
    };

    auto serialize(hemesh const& m, serializer& sr) -> void;

    template <class Serializer, class ...Ts>
    auto serialize(hemesh const& m, Ts &&... xs) -> void
    {
        Serializer sr{std::forward<Ts>(xs)...};
        serialize(m, sr);
    }
}

