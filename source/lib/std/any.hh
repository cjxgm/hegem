#pragma once

#if __has_include(<any>)
    #include <any>
    #define LIB_ANY any
#elif __has_include(<experimental/any>)
    #include <experimental/any>
    #define LIB_ANY experimental::any
#else
    #error "Requires an std::any implementation."
#endif

namespace lib
{
    using std::LIB_ANY;
}

#undef LIB_ANY

