#pragma once

#if __has_include(<optional>)
    #include <optional>
    #define LIB_OPTIONAL optional
#elif __has_include(<experimental/optional>)
    #include <experimental/optional>
    #define LIB_OPTIONAL experimental::optional
#else
    #error "Requires an optional implementation."
#endif

namespace lib
{
    using std::LIB_OPTIONAL;
}

#undef LIB_OPTIONAL

