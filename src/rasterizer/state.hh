#pragma once
#include "../glu/resource.hh"
#include "../scene/scene.hh"
#include "../scene/view.hh"
#include "sort.hh"

namespace rt::rasterizer
{
    namespace state_details
    {
        using scene::scene_type;
        using scene::view_type;

        struct state
        {
            scene_type const& scene;
            view_type view;
            sorted_geometry sg;

            glu::shared_framebuffer fbo_combined;
            glu::shared_framebuffer fbo_geometry;

            glu::shared_renderbuffer depth;
            glu::shared_texture2d albedo;
            glu::shared_texture2d normal;
            glu::shared_texture2d position;
            glu::shared_texture2d material;

            glu::shared_program prog_sky;
            glu::shared_program prog_sphere;
            glu::shared_program prog_shade;

            glu::shared_vertex_array vao_empty;

            state(scene_type const& scene, view_type view, glu::shared_framebuffer combined);
        };
    }

    using state_details::state;
}

