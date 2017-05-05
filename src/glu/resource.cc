#include "resource.hh"
#include "traits.hh"

namespace rt::glu
{
    inline namespace resource_details
    {
        namespace
        {
            template <gl::enum_type ...Types>
            void init_shader_pools(traits::gl_enum_sequence<Types...>)
            {
                (shader_pool<Types>::instance(), ...);
            }
        }

        int resource_pool_count{};

        void resource_recycler::try_recycle()
        {
            for (auto& f: fs) f();
        }

        resource_recycler& resource_recycler::instance()
        {
            static resource_recycler rr;
            return rr;
        }

        void init_all_resource_pools_once()
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

