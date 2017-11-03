#pragma once
#include "../lib/glm/vec3.hh"
#include "primitive.fwd.hh"

namespace rt::hemesh
{
    inline namespace primitive
    {
        using position_type = glm::vec3;

        // Define structs
        #define STRUCT(NAME, VAR) struct NAME {
        #define END_STRUCT() };
        #define FIELD_PTR_FROM_SLAB(TYPE, VAR) TYPE* VAR;
        #define FIELD(TYPE, VAR) TYPE VAR;
        #include "primitive.inl"
    }
}

