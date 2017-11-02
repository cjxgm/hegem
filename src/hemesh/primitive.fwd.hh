#pragma once

namespace rt::hemesh
{
    inline namespace primitive
    {
        // Forward declarations
        #define STRUCT(NAME) struct NAME;
        #include "primitive.inl"
    }
}

