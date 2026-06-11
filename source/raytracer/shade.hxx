#pragma once
#include "../image/image.hxx"
#include "../image/color.hxx"
#include "../scene/scene.hxx"
#include "../scene/view.hxx"
#include "../math/sampler.hxx"
#include "ray.hxx"
#include "hit.hxx"

namespace hegem::raytracer
{
    namespace shading_details
    {
        using image_type = image::image_rgb;
        using color_type = image::color::linear_rgb;
        using scene::scene_type;
        using scene::view_type;

        color_type shade_environment(scene_type const& scene, ray_type const& viewing);
        color_type shade_diffuse(scene_type const& scene, hits::object const& hit, math::normal_sampler & samp);
        color_type shade_illumination(
                scene_type const& scene,
                hits::object const& hit,
                color_type const& diffuse,
                color_type const& reflected,
                color_type const& refracted);
        #include "shade.impl/environment.pull"
        #include "shade.impl/diffuse.pull"
        #include "shade.impl/illumination.pull"

        image_type shade_depth(hit_buffer_type const& buf, view_type const& view);
        image_type shade_normal(hit_buffer_type const& buf, view_type const& view);
        #include "shade.impl/extra.pull"
    }

    using shading_details::shade_depth;
    using shading_details::shade_normal;
    using shading_details::shade_environment;
    using shading_details::shade_diffuse;
    using shading_details::shade_illumination;
}

