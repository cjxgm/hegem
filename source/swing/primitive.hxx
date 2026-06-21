#pragma once
#include "type.hxx"
#include "primitive.fwd.hxx"

namespace hegem::swing
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

