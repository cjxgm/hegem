#pragma once

namespace rt::hegem
{
    inline namespace primitive
    {
        // Forward declarations
        #define STRUCT(NAME, VAR) struct NAME;
        #include "primitive.inl"
    }
}

