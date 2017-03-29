#pragma once
// unit vector

#define WANT_GEOM_OP
#include "../lib/glm.hh"
#undef WANT_GEOM_OP

namespace rt::math
{
    template <class T>
    struct unit
    {
        using value_type = T;
        using const_pointer = value_type const*;
        using const_reference = value_type const&;

        unit(value_type value) : value{normalize(value)} {}

        auto& get() const { return value; }
        const_reference operator * () const { return get(); }
        operator const_reference () const { return get(); }

    private:
        value_type value;
    };
}

