#pragma once
// This module is about rebuilding hemesh data structure, this can be used to:
//
// - Cloning a hemesh
// - Deserialize hemesh from file
// - Condense/Garbage-collect the slabs
#include "hemesh.fwd.hh"
#include <vector>
#include <unordered_map>
#include <functional>

namespace rt::hemesh
{
    using relocatable_pointer_type = std::reference_wrapper<void*>;
    using pointer_list_type = std::vector<relocatable_pointer_type>;
    using pointer_map_type = std::unordered_map<void const*, void *>;

    pointer_list_type collect_pointers(hemesh & m);
    pointer_map_type build_pointer_map(hemesh const& old, hemesh & now);
    void reconstruct_pointers(pointer_map_type const& pmap, pointer_list_type const& ptrs);
}

