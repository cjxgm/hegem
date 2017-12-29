#include "../lib/gl/gl.hh"
#include "visualization.hh"
#include <utility>      // for std::move

namespace rt::app::visualization_details
{
    visualization::visualization(std::string name, scene_type& scene, view_type& view, bool raytrace)
        : name{std::move(name)}
        , hdr{view.size.x, view.size.y}
        , fbo{glu::framebuffer_pool::instance().allocate()}
        , s{scene, view, fbo}
    {
        gl::named_framebuffer_draw_buffer(fbo, gl::color_attachment0);
        gl::named_framebuffer_texture(fbo, gl::color_attachment0, hdr.tex, 0);

        if (raytrace) {
            show_raytracing_overlay = true;
            suppress_raytracing = 10;
        }
    }

    void visualization::reset_raytracing_task_io()
    {
        if (raytracing_task_io)
            raytracing_task_io->cancel();
        raytracing_task_io.reset();
    }

    void visualization::reset_raytracing_task_io(util::task_io io)
    {
        if (raytracing_task_io)
            raytracing_task_io->cancel();
        raytracing_task_io = std::move(io);
    }

    void visualization::reset_swrast_task_io()
    {
        if (swrast_task_io)
            swrast_task_io->cancel();
        swrast_task_io.reset();
    }

    void visualization::reset_swrast_task_io(util::task_io io)
    {
        if (swrast_task_io)
            swrast_task_io->cancel();
        swrast_task_io = std::move(io);
    }

    void visualization::update_rasterization_state()
    {
        s.update_geometry();
    }
}

