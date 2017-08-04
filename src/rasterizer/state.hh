#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/glm/vec4.hh"
#include "../glu/resource.hh"
#include "../image/color.hh"
#include "../scene/scene.hh"
#include "../scene/view.hh"
#include "../raytracer/ray.hh"
#include "sort.hh"
#include <vector>
#include <array>

namespace rt::rasterizer
{
    namespace state_details
    {
        using scene::scene_type;
        using scene::view_type;
        using color_type = image::color::linear_rgb;
        using raytracer::ray_type;

        struct line_segment
        {
            std::array<glm::vec3, 2> ends;
            std::array<glm::vec4, 2> colors;
            float width;

            line_segment(ray_type ray, float extent, color_type color, float width);
        };

        struct state
        {
            scene_type const& scene;
            view_type view;
            sorted_geometry geometry;
            std::vector<line_segment> segments;

            glu::shared_framebuffer fbo_combined;
            glu::shared_framebuffer fbo_geometry;
            glu::shared_framebuffer fbo_blit_target;

            // G-Buffer:
            // - albedo: vec4(vec3(albedo_color), roughness)
            // - reflection: vec4(vec3(refl_color), ior)
            // - normal: vec4(vec3(normal_dir), 0.0f)
            // - position: vec4(vec3(pos_in_world_space), material_enum)
            //   * material_enum:
            //     [0.0f] solid color
            //     [1.0f] pbr
            //     [2.0f] sky
            glu::shared_renderbuffer depth;
            glu::shared_texture2d albedo;
            glu::shared_texture2d reflection;
            glu::shared_texture2d normal;
            glu::shared_texture2d position;
            glu::shared_texture2d combined;

            glu::shared_program prog_sky;
            glu::shared_program prog_sphere;
            glu::shared_program prog_plane;
            glu::shared_program prog_mesh;
            glu::shared_program prog_box;
            glu::shared_program prog_shade;
            glu::shared_program prog_line_segment;
            glu::shared_program prog_blit;

            glu::shared_buffer static_vertices_buffer;
            glu::shared_buffer static_elements_buffer;

            glu::shared_vertex_array vao_empty;
            glu::shared_vertex_array vao_sphere;

            state(scene_type const& scene, view_type view, glu::shared_framebuffer output);
        };
    }

    using state_details::state;
}

