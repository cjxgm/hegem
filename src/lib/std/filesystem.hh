#pragma once

#if __has_include(<filesystem>)
    #include <filesystem>
    #define LIB_FILESYSTEM filesystem
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
    #define LIB_FILESYSTEM experimental::filesystem
#else
    #error "Requires a filesystem implementation."
#endif

namespace lib
{
    namespace filesystem = std::LIB_FILESYSTEM;
}

#undef LIB_FILESYSTEM

