#include "../gui/wsi.hh"
#include "../scene/parser.hh"
#include "../scene/view.hh"
#include "app.hh"
#include "scene-instance.hh"

namespace rt::app
{
    void run(options const& opts)
    {
        using image::color::linear_rgb;

        auto& s = scene_instance();
        s = scene::from_path(opts.input_path);
        s.views[0] = scene::view_type {
            {opts.width, opts.height},
            s.views[0].camera(),
        };

        gui::wsi::context ctx{"Raytracer"};
        ctx.mainloop();

        #if 0
        auto result = raytracer::raytrace(s, 0, 8);
        auto& image = std::get<0>(result);
        auto& buf = std::get<1>(result);

        if (!opts.output_path.empty()) {
            write(to_srgb(tonemap(
                            image,
                            linear_rgb{0},
                            linear_rgb{10})),
                    opts.output_path);
        }
        if (!opts.depth_path.empty()) {
            auto depth = raytracer::shade_depth(buf, s, 0);
            write(to_srgb(tonemap(
                            depth,
                            linear_rgb{opts.depth_max},
                            linear_rgb{opts.depth_min})),
                    opts.depth_path);
        }
        if (!opts.normal_path.empty()) {
            auto normal = raytracer::shade_normal(buf, s, 0);
            write(to_srgb(normal), opts.normal_path);
        }
        #endif
    }
}

