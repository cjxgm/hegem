#include "../lib/gl/gl.hh"
#include "visualization.hh"
#include <utility>      // for std::move

namespace rt::app::visualization_details
{
    visualization::visualization(std::string name, scene_type& scene, view_type& view)
        : name{std::move(name)}
        , hdr{view.size.x, view.size.y}
        , fbo{glu::framebuffer_pool::instance().allocate()}
        , s{scene, view, fbo}
    {
        gl::named_framebuffer_draw_buffer(fbo, gl::color_attachment0);
        gl::named_framebuffer_texture(fbo, gl::color_attachment0, hdr.tex, 0);
    }
}

