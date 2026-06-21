#pragma once
#include "../lib/gl/gl.hxx"
#include <cstdint>

namespace hegem::glu::cast
{
    inline void* id_to_ptr(gl::uint_type id) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(id)); }
    inline gl::uint_type ptr_to_id(void* ptr) { return static_cast<gl::uint_type>(reinterpret_cast<std::uintptr_t>(ptr)); }
}

