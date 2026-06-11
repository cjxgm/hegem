#pragma once
#include "../lib/gl/gl.hxx"
#include <vector>
#include <utility>  // for std::forward
#include <functional>

namespace hegem::glu
{
    inline namespace resource_details
    {
        template <class T>
        struct resource_traits;
        // using id_type = ...;
        // static void bulk_allocate(int n, id_type* xs);
        // static void bulk_delete(int n, id_type* xs);
        // static void recycle(id_type& x);
        // static constexpr auto prefered_bulk_allocation_size();
        // static constexpr auto name();

        extern int resource_pool_count;

        template <class T, class Traits = resource_traits<T>>
        struct resource_pool;

        template <class T, class Traits = resource_traits<T>>
        struct shared_resource;

        struct resource_recycler
        {
            template <class T, class Traits>
            friend struct resource_pool;

            // should be called every frame
            void try_recycle();
            static resource_recycler& instance();

        private:
            std::vector<std::function<void()>> fs;

            template <class F>
            void register_try_recycle_fn(F&& f)
            {
                fs.emplace_back(std::forward<F>(f));
            }
        };

        template <class T>
        struct make_resource_traits
        {
            using id_type = gl::uint_type;
            using traits = resource_traits<T>;
            static constexpr auto preferred_bulk_allocation_size() { return 8; }

            static void recycle(id_type& x)
            {
                traits::bulk_delete(1, &x);
                traits::bulk_allocate(1, &x);
            }
        };


        #define KP(X...) X      // kill parenthesis
        #define TEMPLATE_TYPE(TPARAM...)
        #define TEMPLATE_TYPEtemplate(TPARAM...) template <TPARAM>
        #define TEMPLATE_TRAITS(TPARAM...) template<>
        #define TEMPLATE_TRAITStemplate(TPARAM...) template <TPARAM>
        #define TEMPLATE_ARGS(TARGS...)
        #define TEMPLATE_ARGStemplate(TARGS...) <TARGS>
        #define DEFINE_RESOURCE_FULL(TEMPLATED, TPARAM, TARG, TYPE, ALLOC, DEALLOC) \
            TEMPLATE_TYPE ## TEMPLATED TPARAM \
            struct TYPE; \
            TEMPLATE_TRAITS ## TEMPLATED TPARAM \
            struct resource_traits<TYPE TEMPLATE_ARGS ## TEMPLATED TARG> \
                : make_resource_traits<TYPE TEMPLATE_ARGS ## TEMPLATED TARG> \
            { \
                using id_type = typename make_resource_traits<TYPE TEMPLATE_ARGS ## TEMPLATED TARG>::id_type; \
                static constexpr auto name() { return #TYPE; } \
                static void bulk_allocate(int n, id_type* xs) { KP ALLOC } \
                static void bulk_delete(int n, id_type* xs) { KP DEALLOC } \
            }; \
            TEMPLATE_TYPE ## TEMPLATED TPARAM \
            using TYPE ## _pool = resource_pool<TYPE TEMPLATE_ARGS ## TEMPLATED TARG>; \
            TEMPLATE_TYPE ## TEMPLATED TPARAM \
            using shared_ ## TYPE = shared_resource<TYPE TEMPLATE_ARGS ## TEMPLATED TARG>

        #define ALLOC_DSA(ALLOC_FN) (ALLOC_FN(n, xs);)
        #define ALLOC_DSA_EXTRA(ALLOC_FN, EXTRA...) (ALLOC_FN(EXTRA, n, xs);)
        #define DEALLOC_DSA(DEALLOC_FN) (DEALLOC_FN(n, xs);)
        #define ALLOC_NON_BULK(ALLOC_FN, EXTRA...) (while (n--) *xs++ = ALLOC_FN(EXTRA);)
        #define DEALLOC_NON_BULK(DEALLOC_FN) (while (n--) DEALLOC_FN(*xs++);)

        #define DEFINE_RESOURCE(TYPE, ALLOC, DEALLOC) \
            DEFINE_RESOURCE_FULL(, (), (), TYPE, ALLOC, DEALLOC)
        #define DEFINE_RESOURCE_DSA(TYPE, ALLOC_FN, DEALLOC_FN) \
            DEFINE_RESOURCE(TYPE, ALLOC_DSA(ALLOC_FN), DEALLOC_DSA(DEALLOC_FN))
        #define DEFINE_RESOURCE_NON_BULK(TYPE, ALLOC_FN, DEALLOC_FN) \
            DEFINE_RESOURCE(TYPE, ALLOC_NON_BULK(ALLOC_FN), DEALLOC_NON_BULK(DEALLOC_FN))

        #define DEFINE_RESOURCE_TEMPLATED(TTYPE, TARG, TYPE, ALLOC, DEALLOC) \
            DEFINE_RESOURCE_FULL(template, (TTYPE TARG), (TARG), TYPE, ALLOC, DEALLOC)
        #define DEFINE_RESOURCE_TEMPLATED_DSA(TTYPE, TARG, TYPE, ALLOC_FN, DEALLOC_FN) \
            DEFINE_RESOURCE_TEMPLATED(TTYPE, TARG, TYPE, ALLOC_DSA_EXTRA(ALLOC_FN, TARG), DEALLOC_DSA(DEALLOC_FN))
        #define DEFINE_RESOURCE_TEMPLATED_NON_BULK(TTYPE, TARG, TYPE, ALLOC_FN, DEALLOC_FN) \
            DEFINE_RESOURCE_TEMPLATED(TTYPE, TARG, TYPE, ALLOC_NON_BULK(ALLOC_FN, TARG), DEALLOC_NON_BULK(DEALLOC_FN))


        DEFINE_RESOURCE_DSA(vertex_array, gl::create_vertex_arrays, gl::delete_vertex_arrays);
        DEFINE_RESOURCE_DSA(buffer, gl::create_buffers, gl::delete_buffers);
        DEFINE_RESOURCE_DSA(framebuffer, gl::create_framebuffers, gl::delete_framebuffers);
        DEFINE_RESOURCE_DSA(renderbuffer, gl::create_renderbuffers, gl::delete_renderbuffers);
        DEFINE_RESOURCE_NON_BULK(program, gl::create_program, gl::delete_program);

        DEFINE_RESOURCE_TEMPLATED_DSA(
                gl::enum_type, Target,
                texture,
                gl::create_textures, gl::delete_textures);
        using texture2d_pool   = texture_pool<gl::texture_2d>;
        using shared_texture2d = shared_texture<gl::texture_2d>;

        DEFINE_RESOURCE_TEMPLATED_NON_BULK(
                gl::enum_type, Type,
                shader,
                gl::create_shader, gl::delete_shader);

        void init_all_resource_pools_once();


        #undef DEFINE_RESOURCE_TEMPLATED_NON_BULK
        #undef DEFINE_RESOURCE_TEMPLATED_DSA
        #undef DEFINE_RESOURCE_TEMPLATED

        #undef DEFINE_RESOURCE_DSA
        #undef DEFINE_RESOURCE_NON_BULK
        #undef DEFINE_RESOURCE

        #undef DEALLOC_NON_BULK
        #undef ALLOC_NON_BULK
        #undef DEALLOC_DSA
        #undef ALLOC_DSA_EXTRA
        #undef ALLOC_DSA

        #undef DEFINE_RESOURCE_FULL
        #undef TEMPLATE_ARGStemplate
        #undef TEMPLATE_ARGS
        #undef TEMPLATE_TRAITStemplate
        #undef TEMPLATE_TRAITS
        #undef TEMPLATE_TYPEtemplate
        #undef TEMPLATE_TYPE
        #undef KP
    }
}

#ifndef RESOURCE_INCLUDED
#define RESOURCE_INCLUDED
#include "resource.inl"
#undef RESOURCE_INCLUDED
#endif

