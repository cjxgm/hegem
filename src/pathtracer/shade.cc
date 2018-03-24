#include "../raytracer/shade.hh"
#include "../scene/material.hh"
#include "shade.hh"

namespace rt::pathtracer::shading_details
{
    namespace
    {
        namespace hits = raytracer::hits;
        namespace materials = scene::materials;

        struct shader
        {
            hits::object const& hit;

            template <class Material>
            auto operator () (Material const& mat) const -> shading_point
            {
                return impl(mat);
            }

        private:
            shading_point impl(materials::solid_color const& mat) const
            {
                return mat.color;
            }

            shading_point impl(materials::phong const& mat) const
            {
                return {};
            }

            shading_point impl(materials::physically_based const& mat) const
            {
                // TODO
                return mat.albedo;
            }
        };
    }

    auto shade(scene_type const& scene, object_hit_type const& hit, math::normal_sampler & samp) -> shading_point
    {
        return hit.match(
            [&] (hits::missed const& hit) -> shading_point {
                return raytracer::shade_environment(scene, hit.viewing);
            },
            [&] (hits::object const& hit) {
                shader s{hit};
                return apply_visitor(s, scene.materials[hit.material_id]);
            }
        );
    }
}

