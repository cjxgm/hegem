#include "../lib/gl/gl.hh"
#include "../lib/glm/op/transform.hh"
#include "../glu/states.hh"
#include "rasterizer.hh"

namespace rt::rasterizer
{
    namespace
    {
        float const initial_depth = 1;
    }

    void rasterize(state const& s, bool wireframed)
    {
        auto& sm = glu::states_manager::instance();

        gl::funcs::viewport(0, 0, s.view.size.x, s.view.size.y);

        gl::bind_framebuffer(gl::framebuffer, s.fbo_geometry);
        gl::clear_bufferfv(gl::depth, 0, &initial_depth);

        // sky geometry pass
        sm.enable_only({});
        gl::bind_vertex_array(s.vao_empty);
        gl::use_program(s.prog_sky);
        gl::uniform3fv(0, 1, &s.geometry.sky.color[0]);
        gl::draw_arrays(gl::points, 0, 1);

        // shapes geometry pass
        sm.enable_only({ gl::depth_test });
        float aspect_ratio = float(s.view.size.x) / s.view.size.y;
        auto proj_view = world_space_to_clip_space(s.view.camera, aspect_ratio);

        // - mesh based
        if (wireframed) gl::funcs::polygon_mode(gl::front_and_back, gl::line);

        // -- spheres
        gl::bind_vertex_array(s.vao_sphere);
        gl::use_program(s.prog_sphere);
        gl::uniform_matrix4fv(0, 1, false, &proj_view[0][0]);
        gl::patch_parameteri(gl::patch_vertices, 4);
        for (auto& sphere: s.geometry.spheres) {
            auto& mat = s.geometry.materials[sphere.material_id];
            auto& shape = sphere.shape;
            auto model = glm::translate(shape.center) * glm::scale(glm::vec3{shape.radius});
            gl::uniform_matrix4fv(1, 1, false, &model[0][0]);
            gl::uniform3fv(2, 1, &mat.albedo[0]);
            gl::draw_elements(gl::patches, 6*4, gl::unsigned_int, nullptr);
        }

        // -- triangle meshes

        if (wireframed) gl::funcs::polygon_mode(gl::front_and_back, gl::fill);

        // - non mesh based

        // -- planes

        // -- line segments

        // shading pass
        sm.enable_only({});
        auto cam_apex = apex_in_world_space(s.view.camera, aspect_ratio);
        gl::bind_framebuffer(gl::framebuffer, s.fbo_combined);
        gl::bind_vertex_array(s.vao_empty);
        gl::bind_texture_unit(0, s.albedo);
        gl::bind_texture_unit(1, s.normal);
        gl::bind_texture_unit(2, s.position);
        gl::bind_texture_unit(3, s.material);
        gl::use_program(s.prog_shade);
        gl::uniform3fv(4, 1, &cam_apex[0]);
        gl::uniform3fv(5, 1, &s.geometry.sky.color[0]);
        gl::draw_arrays(gl::points, 0, 1);
    }
}

