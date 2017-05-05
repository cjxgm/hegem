#include "../lib/gl/gl.hh"
#include "../glu/states.hh"
#include "rasterizer.hh"

namespace rt::rasterizer
{
    namespace
    {
        float const initial_depth = 1;
    }

    void rasterize(state const& s)
    {
        auto& sm = glu::states_manager::instance();

        gl::funcs::viewport(0, 0, s.view.size.x, s.view.size.y);

        gl::bind_framebuffer(gl::framebuffer, s.fbo_geometry);
        gl::clear_bufferfv(gl::depth, 0, &initial_depth);

        // sky geometry pass
        gl::bind_vertex_array(s.vao_empty);
        gl::use_program(s.prog_sky);
        gl::uniform3fv(0, 1, &s.sg.sky.color[0]);
        sm.enable_only({});
        gl::draw_arrays(gl::points, 0, 1);

        // shapes geometry pass
        gl::use_program(s.prog_sphere);
        sm.enable_only({ gl::depth });

        // shading pass
        gl::bind_framebuffer(gl::framebuffer, s.fbo_combined);
        gl::bind_vertex_array(s.vao_empty);
        gl::use_program(s.prog_shade);
        sm.enable_only({});
        gl::draw_arrays(gl::points, 0, 1);
    }
}

