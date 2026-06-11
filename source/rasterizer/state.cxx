#include "../lib/gl/gl.hxx"
#include "../glu/shader.hxx"
#include "../math/constants.hxx"
#include "state.hxx"
#include <utility>      // for std::move

namespace hegem::rasterizer::state_details
{
    namespace
    {
        using math::inf;

        auto default_texture2d(int w, int h, gl::enum_type internal_format=gl::rgba32f)
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
            glu::shared_program prog_plane{glu::shader_factory::program_from_name("g-plane")};
            glu::shared_program prog_mesh{glu::shader_factory::program_from_name("g-mesh")};
            glu::shared_program prog_box{glu::shader_factory::program_from_name("g-box")};
            glu::shared_program prog_shade{glu::shader_factory::program_from_name("shade-gbuffer")};
            glu::shared_program prog_line_segment{glu::shader_factory::program_from_name("line-segment")};
            glu::shared_program prog_blit{glu::shader_factory::program_from_name("blit")};

            glu::shared_buffer static_vertices_buffer;
            glu::shared_buffer static_elements_buffer;

            glu::shared_vertex_array vao_empty;
            glu::shared_vertex_array vao_sphere;

            static auto& instance()
            {
                static context ctx;
                return ctx;
            }

        private:
            context()
            {
                gl::program_uniform1i(prog_shade, 0, 0);
                gl::program_uniform1i(prog_shade, 1, 1);
                gl::program_uniform1i(prog_shade, 2, 2);
                gl::program_uniform1i(prog_shade, 3, 3);
                gl::program_uniform1i(prog_blit, 0, 0);

                auto& vao_pool = glu::vertex_array_pool::instance();
                auto& buf_pool = glu::buffer_pool::instance();
                vao_empty = vao_pool.allocate();
                vao_sphere = vao_pool.allocate();
                static_vertices_buffer = buf_pool.allocate();
                static_elements_buffer = buf_pool.allocate();

                gl::enable_vertex_array_attrib(vao_sphere, 0);  // vec3 pos
                gl::vertex_array_attrib_format(
                        vao_sphere, 0,
                        3, gl::float_, false,
                        0);
                gl::vertex_array_vertex_buffer(vao_sphere, 0, static_vertices_buffer, 0, sizeof(glm::vec3));
                gl::vertex_array_element_buffer(vao_sphere, static_elements_buffer);

                static glm::vec3 vertices[] = {
                    // cube
                    { -1, -1, -1 },     // 0 far
                    {  1, -1, -1 },     // 1
                    {  1,  1, -1 },     // 2
                    { -1,  1, -1 },     // 3
                    { -1, -1,  1 },     // 4 near
                    {  1, -1,  1 },     // 5
                    {  1,  1,  1 },     // 6
                    { -1,  1,  1 },     // 7
                };
                static unsigned int elements[] = {
                    // cube patches
                    0, 3, 2, 1,         // far
                    4, 5, 6, 7,         // near
                    0, 1, 5, 4,         // bottom
                    3, 7, 6, 2,         // top
                    0, 4, 7, 3,         // left
                    1, 2, 6, 5,         // right
                };
                gl::named_buffer_data(
                    static_vertices_buffer,
                    sizeof(vertices),
                    vertices,
                    gl::static_draw);
                gl::named_buffer_data(
                    static_elements_buffer,
                    sizeof(elements),
                    elements,
                    gl::static_draw);
            }
        };
    }

    line_segment::line_segment(ray_type ray, float extent, color_type color, float width)
        : ends{
            ray.origin,
            ray.at(extent > 10.0f ? 1.0f : extent),
        }
        , colors{
            glm::vec4{color, 1.0f},
            glm::vec4{color * 0.3f, extent > 10.0f ? 0.0f : 0.2f},
        }
        , width{width}
    {
    }

    state::state(scene_type const& scene, view_type view, glu::shared_framebuffer output)
        : scene{scene}
        , view{view}
        , geometry{sort_geometry(scene)}
        , fbo_blit_target{std::move(output)}
    {
        auto w = view.size.x;
        auto h = view.size.y;

        depth = glu::renderbuffer_pool::instance().allocate();
        gl::named_renderbuffer_storage(depth, gl::depth_component32f, w, h);

        albedo     = default_texture2d(w, h);
        reflection = default_texture2d(w, h);
        normal     = default_texture2d(w, h);
        position   = default_texture2d(w, h);
        combined   = default_texture2d(w, h);

        {
            fbo_combined = glu::framebuffer_pool::instance().allocate();
            gl::named_framebuffer_texture(fbo_combined, gl::color_attachment0, combined, 0);
            gl::named_framebuffer_renderbuffer(fbo_combined, gl::depth_attachment, gl::renderbuffer, depth);
            gl::enum_type draw_buffers[] = {
                gl::color_attachment0,
            };
            constexpr auto draw_buffers_count = sizeof(draw_buffers)/sizeof(*draw_buffers);
            gl::named_framebuffer_draw_buffers(fbo_combined, draw_buffers_count, draw_buffers);
        }

        {
            fbo_geometry = glu::framebuffer_pool::instance().allocate();
            gl::named_framebuffer_texture(fbo_geometry, gl::color_attachment0, albedo, 0);
            gl::named_framebuffer_texture(fbo_geometry, gl::color_attachment1, reflection, 0);
            gl::named_framebuffer_texture(fbo_geometry, gl::color_attachment2, normal, 0);
            gl::named_framebuffer_texture(fbo_geometry, gl::color_attachment3, position, 0);
            gl::named_framebuffer_renderbuffer(fbo_geometry, gl::depth_attachment, gl::renderbuffer, depth);
            gl::enum_type draw_buffers[] = {
                gl::color_attachment0,
                gl::color_attachment1,
                gl::color_attachment2,
                gl::color_attachment3,
            };
            constexpr auto draw_buffers_count = sizeof(draw_buffers)/sizeof(*draw_buffers);
            gl::named_framebuffer_draw_buffers(fbo_geometry, draw_buffers_count, draw_buffers);
        }

        auto& ctx = context::instance();
        prog_sky = ctx.prog_sky;
        prog_sphere = ctx.prog_sphere;
        prog_plane = ctx.prog_plane;
        prog_mesh = ctx.prog_mesh;
        prog_box = ctx.prog_box;
        prog_shade = ctx.prog_shade;
        prog_line_segment = ctx.prog_line_segment;
        prog_blit = ctx.prog_blit;
        vao_empty = ctx.vao_empty;
        vao_sphere = ctx.vao_sphere;
        static_vertices_buffer = ctx.static_vertices_buffer;
        static_elements_buffer = ctx.static_elements_buffer;
    }

    void state::update_geometry()
    {
        geometry = sort_geometry(scene);
    }
}

