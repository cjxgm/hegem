#include "resource.hxx"
#include "traits.hxx"

namespace hegem::glu
{
    inline namespace resource_details
    {
        inline namespace
        {
            template <gl::enum_type ...Types>
            auto init_shader_pools(traits::gl_enum_sequence<Types...>) -> void
            {
                (shader_pool<Types>::instance(), ...);
            }
        }

        int resource_pool_count{};

        auto resource_recycler::try_recycle() -> void
        {
            for (auto& f: fs) f();
        }

        resource_recycler& resource_recycler::instance()
        {
            static resource_recycler rr;
            return rr;
        }

        auto init_all_resource_pools_once() -> void
        {
            vertex_array_pool::instance();
            buffer_pool::instance();
            framebuffer_pool::instance();
            renderbuffer_pool::instance();
            program_pool::instance();
            texture2d_pool::instance();
            init_shader_pools(traits::shader_types{});
        }
    }
}

