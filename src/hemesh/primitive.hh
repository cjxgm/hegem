#pragma once
#include "../lib/glm/vec3.hh"
#include "primitive.fwd.hh"

namespace rt::hemesh
{
    inline namespace primitive
    {
        using position_type = glm::vec3;

        // Define structs
        #define STRUCT(NAME) struct NAME {
        #define END_STRUCT(VAR) };
        #define FIELD_PTR(TYPE, VAR) TYPE* VAR;
        #define FIELD(TYPE, VAR) TYPE VAR;
        #include "primitive.inl"
    }
}

