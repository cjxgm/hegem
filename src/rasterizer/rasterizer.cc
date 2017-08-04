#include "../lib/gl/gl.hh"
#include "../lib/glm/op/transform.hh"
#include "../lib/glm/op/geom.hh"
#include "../glu/states.hh"
#include "rasterizer.hh"

namespace rt::rasterizer
{
    namespace
    {
        float const initial_depth = 1;
    }

    void rasterize(state const& s, bool wireframed, float time)
    {
        auto& sm = glu::states_manager::instance();
        float aspect_ratio = float(s.view.size.x) / s.view.size.y;
        auto proj_view = world_space_to_clip_space(s.view.camera, aspect_ratio);
        auto cam_apex = apex_in_world_space(s.view.camera, aspect_ratio);
        auto pixel_size = glm::vec2{1.0f} / glm::vec2{s.view.size};

        gl::funcs::viewport(0, 0, s.view.size.x, s.view.size.y);

        gl::bind_framebuffer(gl::framebuffer, s.fbo_geometry);
        gl::clear_bufferfv(gl::depth, 0, &initial_depth);

        // sky geometry pass
        sm.enable_only({});
        gl::bind_vertex_array(s.vao_empty);
        gl::use_program(s.prog_sky);
        gl::uniform3fv(0, 1, &s.geometry.sky.color[0]);
        gl::uniform_matrix4fv(1, 1, false, &proj_view[0][0]);
        gl::draw_arrays(gl::points, 0, 1);

        // shapes geometry pass
        sm.enable_only({ gl::depth_test });

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
            auto model = sphere.model_to_world * glm::translate(shape.center) * glm::scale(glm::vec3{shape.radius});
            gl::uniform_matrix4fv(1, 1, false, &model[0][0]);
            gl::uniform3fv(2, 1, &mat.albedo[0]);
            gl::uniform3fv(3, 1, &mat.reflection[0]);
            gl::uniform1f(4, mat.roughness);
            gl::uniform1f(5, mat.ior);
            gl::draw_elements(gl::patches, 6*4, gl::unsigned_int, nullptr);
        }

        // -- triangle meshes
        gl::use_program(s.prog_mesh);
        gl::uniform_matrix4fv(0, 1, false, &proj_view[0][0]);
        for (auto& mesh: s.geometry.meshes) {
            auto& mat = s.geometry.materials[mesh.material_id];
            auto& shape = mesh.shape;
            auto model = mesh.model_to_world;
            gl::uniform_matrix4fv(1, 1, false, &model[0][0]);
            gl::uniform3fv(2, 1, &mat.albedo[0]);
            gl::uniform3fv(3, 1, &mat.reflection[0]);
            gl::uniform1f(4, mat.roughness);
            gl::uniform1f(5, mat.ior);
            gl::bind_vertex_array(shape.vao);
            gl::draw_elements(gl::triangles, shape.element_count, gl::unsigned_int, nullptr);
        }

        if (wireframed) gl::funcs::polygon_mode(gl::front_and_back, gl::fill);

        // - non mesh based

        // -- planes
        if (!wireframed) {
            gl::bind_vertex_array(s.vao_empty);
            gl::use_program(s.prog_plane);
            gl::uniform_matrix4fv(0, 1, false, &proj_view[0][0]);
            gl::uniform3fv(1, 1, &cam_apex[0]);
            for (auto& plane: s.geometry.planes) {
                auto& mat = s.geometry.materials[plane.material_id];
                auto& shape = plane.shape;
                auto normal = (transpose(plane.world_to_model) * glm::vec4{*shape.normal, 0.0f}).xyz();
                auto nlen = length(normal);
                auto offset = shape.offset / nlen;
                normal /= nlen;
                gl::uniform3fv(2, 1, &normal[0]);
                gl::uniform1f(3, offset);
                gl::uniform3fv(4, 1, &mat.albedo[0]);
                gl::uniform3fv(5, 1, &mat.reflection[0]);
                gl::uniform1f(6, mat.roughness);
                gl::uniform1f(7, mat.ior);
                gl::draw_arrays(gl::points, 0, 1);
            }
        }

        // shading pass
        sm.enable_only({});
        gl::bind_framebuffer(gl::framebuffer, s.fbo_combined);
        gl::bind_vertex_array(s.vao_empty);
        gl::bind_texture_unit(0, s.albedo);
        gl::bind_texture_unit(1, s.reflection);
        gl::bind_texture_unit(2, s.normal);
        gl::bind_texture_unit(3, s.position);
        gl::use_program(s.prog_shade);
        gl::uniform3fv(4, 1, &cam_apex[0]);
        gl::uniform3fv(5, 1, &s.geometry.sky.color[0]);
        gl::uniform1i(6, s.geometry. sun_lamp.colors.size());
        gl::uniform1i(7, s.geometry.omni_lamp.colors.size());
        gl::uniform3fv(32, s.geometry.sun_lamp.dirs.size(), &s.geometry.sun_lamp.dirs[0]->x);
        gl::uniform3fv(64, s.geometry.sun_lamp.colors.size(), &s.geometry.sun_lamp.colors[0][0]);
        gl::uniform3fv(96, s.geometry.omni_lamp.centers.size(), &s.geometry.omni_lamp.centers[0][0]);
        gl::uniform3fv(128, s.geometry.omni_lamp.colors.size(), &s.geometry.omni_lamp.colors[0][0]);
        gl::draw_arrays(gl::points, 0, 1);

        // forward rendering
        // - line segments
        sm.enable_only({ gl::depth_test, gl::blend });
        gl::blend_equation(gl::func_add);
        gl::blend_func(gl::src_alpha, gl::one_minus_src_alpha);
        gl::depth_mask(false);
        gl::bind_framebuffer(gl::framebuffer, s.fbo_combined);
        gl::bind_vertex_array(s.vao_empty);
        gl::use_program(s.prog_line_segment);
        gl::uniform_matrix4fv(0, 1, false, &proj_view[0][0]);
        gl::uniform2fv(1, 1, &pixel_size[0]);
        for (auto& seg: s.segments) {
            gl::uniform3fv(2, 2, &seg.ends[0][0]);
            gl::uniform4fv(4, 2, &seg.colors[0][0]);
            gl::uniform1f(6, seg.width);
            gl::draw_arrays(gl::points, 0, 1);
        }
        gl::depth_mask(true);

        // - voxel boxes
        sm.enable_only({ gl::depth_test, gl::blend });
        gl::blend_equation(gl::func_add);
        gl::blend_func(gl::src_alpha, gl::one);
        gl::depth_mask(false);
        gl::bind_framebuffer(gl::framebuffer, s.fbo_combined);
        gl::bind_vertex_array(s.vao_empty);
        gl::use_program(s.prog_box);
        gl::uniform_matrix4fv(0, 1, false, &proj_view[0][0]);
        for (auto& voxel: s.geometry.voxels) {
            auto& shape = voxel.shape;
            auto model = voxel.model_to_world;
            auto cell_size = shape.voxelized.cell_size();
            shape.voxelized.each([&] (auto& cell, auto& pos) {
                if (cell.size() == 0) return;

                auto world_pos = shape.voxelized.cell_center_pos_to_world_pos(pos);
                auto model = voxel.model_to_world * glm::translate(world_pos) * glm::scale(cell_size);
                auto hotness = cell.size() / 100.0f;

                gl::uniform_matrix4fv(1, 1, false, &model[0][0]);
                gl::uniform1f(2, hotness);
                gl::draw_arrays(gl::points, 0, 1);
            });
        }
        gl::depth_mask(true);

        // blit to output
        sm.enable_only({});
        gl::bind_framebuffer(gl::framebuffer, s.fbo_blit_target);
        gl::bind_vertex_array(s.vao_empty);
        gl::bind_texture_unit(0, s.combined);
        gl::use_program(s.prog_blit);
        gl::draw_arrays(gl::points, 0, 1);
    }
}

