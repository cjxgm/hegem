#pragma once
#include "../image/image.hh"
#include "../scene/scene.hh"
#include "path.hh"

namespace rt::raytracer
{
    namespace shading_details
    {
        using image_type = image::image_rgb;
        using scene::scene_type;

        image_type shade(path_buffer_type const& buf, scene_type const& scene);
        image_type shade_depth(path_buffer_type const& buf, scene_type const& scene, int view_id);
        image_type shade_normal(path_buffer_type const& buf, scene_type const& scene, int view_id);
    }

    using shading_details::shade;
    using shading_details::shade_depth;
    using shading_details::shade_normal;
}

