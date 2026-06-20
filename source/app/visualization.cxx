#include "../lib/gl/gl.hxx"
#include "visualization.hxx"
#include <utility>      // for std::move

namespace hegem::app::visualization_details
{
    visualization::visualization(std::string name, scene_type& scene, view_type& view, bool trace)
        : name{std::move(name)}
        , hdr{view.size.x, view.size.y}
        , fbo{glu::framebuffer_pool::instance().allocate()}
        , s{scene, view, fbo}
    {
        gl::named_framebuffer_draw_buffer(fbo, gl::color_attachment0);
        gl::named_framebuffer_texture(fbo, gl::color_attachment0, hdr.tex, 0);

        if (trace) {
            show_raytracing_overlay = true;
            suppress_raytracing = 10;
        }
    }

    auto visualization::reset_raytracing_task_io() -> void
    {
        if (raytracing_task_io)
            raytracing_task_io->cancel();
        raytracing_task_io.reset();
    }

    auto visualization::reset_raytracing_task_io(tool::task_io io) -> void
    {
        if (raytracing_task_io)
            raytracing_task_io->cancel();
        raytracing_task_io = std::move(io);
    }

    auto visualization::reset_swrast_task_io() -> void
    {
        if (swrast_task_io)
            swrast_task_io->cancel();
        swrast_task_io.reset();
    }

    auto visualization::reset_swrast_task_io(tool::task_io io) -> void
    {
        if (swrast_task_io)
            swrast_task_io->cancel();
        swrast_task_io = std::move(io);
    }

    auto visualization::update_rasterization_state() -> void
    {
        s.update_geometry();
    }
}

