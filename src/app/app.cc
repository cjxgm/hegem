#include "app.hh"
#include "../image/image.hh"

namespace rt::app
{
    void run(options const& opts)
    {
        using image::image_rgb;

        if (!opts.output_path.empty()) {
            image_rgb composite{{opts.width, opts.height}, {0.1, 0.4, 0.8}};
            write(to_srgb(composite), opts.output_path);
        }
        if (!opts.diffuse_path.empty()) {
            image_rgb diffuse{{opts.width, opts.height}, {0.8, 0.4, 0.1}};
            write(to_srgb(diffuse), opts.diffuse_path);
        }
        if (!opts.depth_path.empty()) {
            image_rgb depth{{opts.width, opts.height}, {0.3, 0.3, 0.3}};
            write(to_srgb(depth), opts.depth_path);
        }
    }
}

