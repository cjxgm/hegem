#pragma once
#include "../lib/glm/op/geom.hxx"

namespace hegem::math
{
    // unit vector
    template <class T>
    struct unit
    {
        using value_type = T;
        using const_pointer = value_type const*;
        using const_reference = value_type const&;

        unit() = default;
        unit(value_type value) : value{normalize(value)} {}

        auto& get() const { return value; }
        const_pointer operator -> () const { return &get(); }
        const_reference operator * () const { return get(); }
        operator const_reference () const { return get(); }

    private:
        value_type value;
    };
}

