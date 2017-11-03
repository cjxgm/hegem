#pragma once
#include <string>
#include "hemesh.fwd.hh"
#include "primitive.hh"

namespace rt::hemesh
{
    struct serializer
    {
        virtual void begin_structure(char const* name, void const* ptr) = 0;
        virtual void end_structure() = 0;
        virtual void field_ptr_from_slab(char const* name, void const* ptr) = 0;
        virtual void field(char const* name, position_type pos) = 0;
    };

    void serialize(hemesh const& m, serializer& sr);
}

