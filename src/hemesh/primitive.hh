#pragma once
#include "type.hh"
#include "primitive.fwd.hh"

namespace rt::hemesh
{
    inline namespace primitive
    {
        // Define structs
        #define STRUCT(NAME, VAR) struct NAME {
        #define END_STRUCT() };
        #define FIELD_PTR_FROM_SLAB(TYPE, VAR) TYPE* VAR;
        #define FIELD(TYPE, VAR) TYPE VAR;
        #include "primitive.inl"
    }
}

