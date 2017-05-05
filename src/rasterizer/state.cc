#include "../lib/gl/gl.hh"
#include "../glu/shader.hh"
#include "state.hh"
#include <utility>      // for std::move

namespace rt::rasterizer::state_details
{
    namespace
    {
        auto default_texture2d(int w, int h, gl::enum_type internal_format=gl::rgb32f)
        {
            auto tex = glu::texture2d_pool::instance().allocate();
            gl::texture_storage2d(tex, 1, internal_format, w, h);
            gl::texture_parameteri(tex, gl::texture_min_filter, gl::nearest);
            gl::texture_parameteri(tex, gl::texture_mag_filter, gl::nearest);
            return tex;
        }

        struct context
        {
            glu::shared_program prog_sky{glu::shader_factory::program_from_name("g-sky")};
            glu::shared_program prog_sphere{glu::shader_factory::program_from_name("g-sphere")};
            glu::shared_program prog_shade{glu::shader_factory::program_from_name("shade-gbuffer")};

            static auto& instance()
            {
                static context ctx;
                return ctx;
            }

        private:
            context() = default;
        };
    }

    state::state(scene_type const& scene, view_type view, glu::shared_framebuffer combined)
        : scene{scene}
        , view{view}
        , sg{sort_geometry(scene)}
        , fbo_combined{std::move(combined)}
    {
        auto w = view.size.x;
        auto h = view.size.y;

        depth = glu::renderbuffer_pool::instance().allocate();
        gl::named_renderbuffer_storage(depth, gl::depth_component32f, w, h);

        albedo = default_texture2d(w, h);
        normal = default_texture2d(w, h);
        position = default_texture2d(w, h);
        material = default_texture2d(w, h, gl::r8ui);

        fbo_geometry = glu::framebuffer_pool::instance().allocate();
        gl::named_framebuffer_texture(fbo_geometry, gl::color_attachment0, albedo, 0);
        gl::named_framebuffer_texture(fbo_geometry, gl::color_attachment1, normal, 0);
        gl::named_framebuffer_texture(fbo_geometry, gl::color_attachment2, position, 0);
        gl::named_framebuffer_texture(fbo_geometry, gl::color_attachment3, material, 0);
        gl::named_framebuffer_renderbuffer(fbo_geometry, gl::depth_attachment, gl::renderbuffer, depth);
        gl::enum_type draw_buffers[] = {
            gl::color_attachment0,
            gl::color_attachment1,
            gl::color_attachment2,
            gl::color_attachment3,
        };
        constexpr auto draw_buffers_count = sizeof(draw_buffers)/sizeof(*draw_buffers);
        gl::named_framebuffer_draw_buffers(fbo_geometry, draw_buffers_count, draw_buffers);

        auto& ctx = context::instance();
        prog_sky = ctx.prog_sky;
        prog_sphere = ctx.prog_sphere;
        prog_shade = ctx.prog_shade;

        vao_empty = glu::vertex_array_pool::instance().allocate();
    }
}

