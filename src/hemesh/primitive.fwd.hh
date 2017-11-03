#pragma once

namespace rt::hemesh
{
    inline namespace primitive
    {
        // Forward declarations
        #define STRUCT(NAME, VAR) struct NAME;
        #include "primitive.inl"
    }
}

