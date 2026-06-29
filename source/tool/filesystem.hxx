#pragma once
#include "string.hxx"

namespace hegem::tool
{
    enum struct file_kind
    {
        missing,  // The path is invalid, refers to non-existent file or folder, or you have no read permission to the path.
        folder,   // The path refers to a folder.
        link,     // The path refers to a symbolic link.
        plain,    // None of the above.
    };

    // Invariant: assert(filename.find_first_any_of<'\x00', '/'>() == filename.length);
    // Invariant: assert(kind != file_kind::missing);
    // The filename is only available before returning.
    // The filename is mutable.
    using find_file_callback = auto (void* context, tool::string filename, tool::file_kind kind) -> bool;  // -> found.

    // Empty maybe_path is considered as missing.
    auto probe_file_kind(tool::string maybe_path) -> file_kind;

    // On succees, invoke find_file() exactly once. Do nothing on failure.
    // If there existed an invocation of find_file() that returns true, stop immediately and return true. Otherwise, return false.
    // Invariant: if invoking find_file(context, filename, kind), assert(kind != file_kind::link);
    auto probe_real_path(tool::string maybe_path, find_file_callback* find_file, void* context) -> bool;  // -> found.

    // Search a folder for its direct children.
    // If a direct child is a symbolic link, it will be reported as a file_kind::link, instead of the kind that the link refers to.
    // If the folder cannot be listed (empty path, regular file, link to regular file, no permissions, et al), it is considered as an empty folder.
    // If maybe_folder points to a symlink, it will be resolved first.
    // If there existed an invocation of find_file() that returns true, stop immediately and return true. Otherwise, return false.
    // Files are sorted in that file_kind::folder comes first, and then accending by filenames in byte-wise lexicographical order, assuming every filename ends with a virtual '\x00' byte.
    auto search_folder(tool::string maybe_folder, find_file_callback* find_file, void* context) -> bool;  // -> found.
}

