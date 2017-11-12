#pragma once
// This module is about extracting information from the meta system
// for rebuilding and querying the hemesh data structure.
//
// These information can be used to:
// - Clone a hemesh
// - Serialize/deserialize hemesh
// - Condense/defragment the slabs
// - Fsck
#include "hemesh.fwd.hh"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace rt::hegem
{
    inline namespace meta
    {
        using relocatable_pointer_type = std::reference_wrapper<void*>;
        using pointer_list_type = std::vector<relocatable_pointer_type>;
        using pointer_set_type = std::unordered_set<void const*>;
        using pointer_map_type = std::unordered_map<void const*, void *>;
        using pointer_name_map_type = std::unordered_map<void const*, std::string>;

        pointer_list_type collect_pointers(hemesh & m);
        pointer_name_map_type build_pointer_name_map(hemesh const& m);
        pointer_set_type build_free_pointer_set(hemesh const& m);
        pointer_map_type build_pointer_map(hemesh const& old, hemesh & now);
        void reconstruct_pointers(pointer_map_type const& pmap, pointer_list_type const& ptrs);
    }
}

