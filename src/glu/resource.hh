#pragma once
#include "../lib/gl/gl.hh"
#include <cstddef>
#include <cstdint>
#include <memory>

namespace rt::glu
{
    namespace resource_details
    {
        struct resource_pointer
        {
            resource_pointer(std::nullptr_t = nullptr) : id{} {}
            resource_pointer(gl::uint_type id) : id{id} {}

            explicit operator bool () const { return id != 0; }
            operator gl::uint_type () const { return id; }

            auto get() const { return id; }
            auto operator * () const { return get(); }
            void* ptr() const { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(get())); }

            friend bool operator == (resource_pointer a, resource_pointer b)
            {
                return a.id == b.id;
            }

            friend bool operator != (resource_pointer a, resource_pointer b)
            {
                return !(a == b);
            }

        private:
            gl::uint_type id;
        };

        template <class StaticDestroyer>
        struct resource_deleter
        {
            using pointer = resource_pointer;
            void operator () (pointer ptr) const
            {
                StaticDestroyer::destroy(ptr.get());
            }
        };

        template <class StaticDestroyer>
        using unique_resource = std::unique_ptr<
            gl::uint_type,
            resource_deleter<StaticDestroyer>
        >;
    }

    namespace resource_destroyer
    {
        struct program
        {
            static void destroy(gl::uint_type id) { gl::delete_program(id); }
        };

        struct texture
        {
            static void destroy(gl::uint_type id) { gl::delete_textures(1, &id); }
        };

        struct vertex_array
        {
            static void destroy(gl::uint_type id) { gl::delete_vertex_arrays(1, &id); }
        };

        struct buffer
        {
            static void destroy(gl::uint_type id) { gl::delete_buffers(1, &id); }
        };
    }

    using resource_details::unique_resource;

    using program = unique_resource<resource_destroyer::program>;
    using texture = unique_resource<resource_destroyer::texture>;
    using vertex_array = unique_resource<resource_destroyer::vertex_array>;
    using buffer = unique_resource<resource_destroyer::buffer>;
}

