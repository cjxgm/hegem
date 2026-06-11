#pragma once
#include "../../scene/lamp.hxx"
#include "../../math/sampler.hxx"
#include "../shade.hxx"

namespace hegem::raytracer::shading_details
{
    namespace unified_lamp_details
    {
        struct unified_lamp
        {
            color_type received_radiance;
            ray_type towards_lamp;
            float distance_to_lamp;
        };

        unified_lamp unify_lamp(scene::lamp_type const& lamp, hits::object const& hit, math::normal_sampler & samp);
    }

    using unified_lamp_details::unified_lamp;
    using unified_lamp_details::unify_lamp;
}

