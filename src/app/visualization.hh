#include "../glu/resource.hh"
#include "../lib/std/optional.hh"
#include "../scene/scene.hh"
#include "../scene/view.hh"
#include "../rasterizer/state.hh"
#include "../util/task-manager.hh"
#include "hdr-texture.hh"
#include <string>

namespace rt::app
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

            lib::optional<util::task_io> raytracing_task_io;
            int suppress_raytracing{};
            bool show_raytracing_overlay{false};
            bool trace_path{false};

            lib::optional<util::task_io> swrast_task_io;
            int suppress_swrast{};
            bool show_swrast_overlay{false};

            bool wireframed{false};
            bool with_gizmo{false};
            bool show{true};

            visualization(std::string name, scene_type& scene, view_type& view, bool trace);
            void reset_raytracing_task_io();
            void reset_raytracing_task_io(util::task_io io);
            void reset_swrast_task_io();
            void reset_swrast_task_io(util::task_io io);
            void update_rasterization_state();
        };
    }

    using visualization_details::visualization;
}

