#include "../glu/resource.hxx"
#include "../lib/std/optional.hxx"
#include "../scene/scene.hxx"
#include "../scene/view.hxx"
#include "../rasterizer/state.hxx"
#include "../tool/task-manager.hxx"
#include "hdr-texture.hxx"
#include <string>

namespace hegem::app
{
    namespace visualization_details
    {
        using scene::scene_type;
        using scene::view_type;

        struct visualization
        {
            std::string name;
            hdr_texture hdr;
            glu::shared_framebuffer fbo;
            rasterizer::state s;

            lib::optional<tool::task_io> raytracing_task_io;
            int suppress_raytracing{};
            bool show_raytracing_overlay{false};
            bool trace_path{false};

            lib::optional<tool::task_io> swrast_task_io;
            int suppress_swrast{};
            bool show_swrast_overlay{false};

            bool wireframed{false};
            bool with_gizmo{false};
            bool show{true};

            visualization(std::string name, scene_type& scene, view_type& view, bool trace);
            void reset_raytracing_task_io();
            void reset_raytracing_task_io(tool::task_io io);
            void reset_swrast_task_io();
            void reset_swrast_task_io(tool::task_io io);
            void update_rasterization_state();
        };
    }

    using visualization_details::visualization;
}

