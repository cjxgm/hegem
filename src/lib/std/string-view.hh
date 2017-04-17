#pragma once

#if __has_include(<string_view>)
    #include <string_view>
    #define LIB_STRING_VIEW string_view
#elif __has_include(<experimental/string_view>)
    #include <experimental/string_view>
    #define LIB_STRING_VIEW experimental::string_view
#else
    #error "Requires an string_view implementation."
#endif

namespace lib
{
    using std::LIB_STRING_VIEW;
}

#undef LIB_STRING_VIEW

