#include "app.hh"
#include "../image/image.hh"
#include "../scene/parser.hh"
#include "../scene/view.hh"
#include "../raytracer/raytracer.hh"
#include "../raytracer/shade.hh"

namespace rt::app
{
    void run(options const& opts)
    {
        using image::color::linear_rgb;

        auto s = scene::from_path(opts.input_path);
        s.views[0] = scene::view_type {
            {opts.width, opts.height},
            s.views[0].camera(),
        };

        auto buf = raytracer::raytrace(s, 0, 0);

        if (!opts.output_path.empty()) {
            auto composite = raytracer::shade(buf, s);
            write(to_srgb(composite), opts.output_path);
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
    }
}

