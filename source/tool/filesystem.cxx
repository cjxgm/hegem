#include "filesystem.hxx"
#include "guard.hxx"
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <fts.h>

namespace hegem::tool
{
    inline namespace
    {
        // Will never return file_kind::missing.
        auto file_kind_from_st_mode(::mode_t st_mode) -> file_kind
        {
            if (S_ISDIR(st_mode)) return file_kind::folder;
            if (S_ISLNK(st_mode)) return file_kind::link;
            return file_kind::plain;
        }

        auto is_good_fts_entry(unsigned short fts_info) -> bool
        {
            switch (fts_info) {
                default: return true;
                case FTS_DC: return false;
                case FTS_DP: return false;
                case FTS_ERR: return false;
                case FTS_NS: return false;
            }
        }

        auto do_probe_file_kind(char const* path) -> file_kind
        {
            auto status = (struct stat){};
            if (::lstat(path, &status) == 0) {
                return file_kind_from_st_mode(status.st_mode);
            } else {
                return file_kind::missing;
            }
        }

        auto do_probe_real_path(char const* path, find_file_callback* find_file, void* context) -> bool  // -> found.
        {
            if (auto real_path = ::realpath(path, nullptr)) {
                auto real_path_guard = tool::defer(&::free, real_path);
                auto kind = do_probe_file_kind(real_path);
                if (kind != file_kind::missing && kind != file_kind::link) {
                    return find_file(context, tool::string::borrow_sentinel(real_path), kind);
                } else {
                    // Inconsistent result, assume failure.
                    return false;
                }
            } else {
                return false;
            }
        }

        auto do_search_folder(char const* folder, find_file_callback* find_file, void* context) -> bool  // -> found.
        {
            char* path_argv[]{
                (char*) folder,
                nullptr,
            };
            auto compare_entry = +[] (FTSENT const** a, FTSENT const** b) -> int {
                auto a_is_good = (is_good_fts_entry(a[0]->fts_info) ? 1 : 0);
                auto b_is_good = (is_good_fts_entry(b[0]->fts_info) ? 1 : 0);
                if (a_is_good && b_is_good) {
                    auto a_is_folder = (S_ISDIR(a[0]->fts_statp->st_mode) ? 1 : 0);
                    auto b_is_folder = (S_ISDIR(b[0]->fts_statp->st_mode) ? 1 : 0);
                    if (auto order_by_folder = b_is_folder - a_is_folder) {
                        return order_by_folder;
                    } else {
                        return __builtin_strcmp(a[0]->fts_name, b[0]->fts_name);
                    }
                } else {
                    if (a_is_good) return -1;
                    if (b_is_good) return 1;
                    return 0;
                }
            };
            if (auto walker = ::fts_open(path_argv, FTS_PHYSICAL | FTS_COMFOLLOW | FTS_NOCHDIR, compare_entry)) {
                auto walker_guard = tool::defer(tool::canonical_drop<&::fts_close>, walker);
                while (auto entry = ::fts_read(walker)) {
                    if (entry->fts_level == 1 && is_good_fts_entry(entry->fts_info)) {
                        auto path = tool::string::borrow(entry->fts_name, (usize) entry->fts_namelen);
                        auto kind = file_kind_from_st_mode(entry->fts_statp->st_mode);
                        if (find_file(context, path, kind)) {
                            return true;
                        }
                    }
                }
                return false;
            } else {
                return false;
            }
        }
    }

    auto probe_file_kind(tool::string maybe_path) -> file_kind
    {
        if (auto path = maybe_path) {
            auto path0 = path.clone_as<std::string>();
            return do_probe_file_kind(path0.data());
        } else {
            return file_kind::missing;
        }
    }

    auto probe_real_path(tool::string maybe_path, find_file_callback* find_file, void* context) -> bool
    {
        if (auto path = maybe_path) {
            auto path0 = path.clone_as<std::string>();
            return do_probe_real_path(path0.data(), find_file, context);
        } else {
            return false;
        }
    }

    auto search_folder(tool::string maybe_folder, find_file_callback* find_file, void* context) -> bool
    {
        if (auto folder = maybe_folder) {
            auto folder0 = folder.clone_as<std::string>();
            return do_search_folder(folder0.data(), find_file, context);
        } else {
            return false;
        }
    }
}

