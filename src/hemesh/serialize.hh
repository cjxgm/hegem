#pragma once
#include <string>
#include <utility>      // for std::forward
#include "hemesh.fwd.hh"
#include "primitive.hh"

namespace rt::hemesh
{
    struct serializer
    {
        virtual void declare_structure(char const* type, char const* name, void const* ptr) = 0;

        virtual void begin_structure(char const* type, char const* name, void const* ptr) = 0;
        virtual void end_structure() = 0;
        virtual void field_ptr_from_slab(char const* type, char const* name, void const* ptr) = 0;
        virtual void field(char const* type, char const* name, position_type pos) = 0;
    };

    void serialize(hemesh const& m, serializer& sr);

    template <class Serializer, class ...Ts>
    void serialize(hemesh const& m, Ts &&... xs)
    {
        Serializer sr{std::forward<Ts>(xs)...};
        serialize(m, sr);
    }
}

