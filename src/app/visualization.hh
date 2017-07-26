#include "../glu/resource.hh"
#include "../scene/scene.hh"
#include "../scene/view.hh"
#include "../rasterizer/state.hh"
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
            int suppress_raytracing{};
            bool show_raytracing_overlay{false};
            bool wireframed{false};
            bool show{true};

            visualization(std::string name, scene_type& scene, view_type& view, bool raytrace);
        };
    }

    using visualization_details::visualization;
}

