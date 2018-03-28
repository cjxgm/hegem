#include "../lib/glm/op/geom.hh"
#include "../lib/glm/mat3.hh"
#include "../lib/glm/vec2.hh"
#include "../lib/glm/vec3.hh"
#include "../lib/std/optional.hh"
#include "../global/counter.hh"
#include "../math/direction.hh"
#include "../math/sampler.hh"
#include "../math/constants.hh"
#include "raytracer.hh"
#include "intersect.hh"
#include "shade.hh"
#include <cmath>

namespace rt::raytracer::raytracer_details
{
    namespace
    {
        using global::counter;
        using math::direction_type;
        using math::inf;

        static constexpr auto color_primary_ray = color_type{10.0f, 5.0f, 2.0f};
        static constexpr auto width_primary_ray = 10.0f;
        static constexpr auto color_reflective_ray = color_type{10.0f, 2.0f, 5.0f};
        static constexpr auto width_reflective_ray = 8.0f;
        static constexpr auto color_refractive_ray = color_type{5.0f, 2.0f, 10.0f};
        static constexpr auto width_refractive_ray = 8.0f;
        static constexpr auto color_normal_ray = color_type{2.0f, 5.0f, 10.0f};
        static constexpr auto width_normal_ray = 10.0f;
        static constexpr auto length_normal_ray = 0.3f;

        struct shading_point
        {
            object_hit_type hit;
            int reflection;
            int refraction;

            shading_point(hits::missed m)
                : hit{m}
                , reflection{0}
                , refraction{0}
            {}

            shading_point(hits::object hit, int reflection, int refraction)
                : hit{hit}
                , reflection{reflection}
                , refraction{refraction}
            {}
        };

        struct shading_point_pool
        {
            using id_type = int;

            shading_point_pool()
            {
                // setup guard element
                push(hits::missed{});
            }

            id_type push(hits::missed m)
            {
                return push_sp(m);
            }

            id_type push(
                hits::object hit,
                id_type reflection,
                id_type refraction)
            {
                return push_sp(hit, reflection, refraction);
            }

            auto begin() const { return ++pool.begin(); }
            auto end() const { return pool.end(); }
            auto size() const { return pool.size() - 1; }
            auto& operator [] (int i) const { return pool[i]; }

        private:
            using pool_type = std::vector<shading_point>;
            pool_type pool;

            template <class ...Args>
            id_type push_sp(Args... args)
            {
                auto id = static_cast<id_type>(pool.size());
                pool.emplace_back(std::forward<Args>(args)...);
                return id;
            }
        };

        struct raytracer_impl
        {
            scene_type const& scene;

            math::normal_sampler nsamp{0, 0.2};

            int trace_ray(
                ray_type const& ray,
                int remaining_bounces)
            {
                counter.ray++;
                if (remaining_bounces < 0)
                    return spp.push(hits::missed{ ray });
                remaining_bounces--;

                return intersect(scene.cache, ray).match(
                    [&] (hits::missed m) {
                        return spp.push(m);
                    },
                    [&] (hits::object hit) {
                        int reflection{};
                        int refraction{};

                        auto& mat = scene.materials[hit.material_id];
                        if (reflective(mat)) {
                            auto into = (dot(*ray.dir, *hit.shape_info.normal) < 0.0f);
                            auto refl_normal = (into ? 1.0f : -1.0f) * *hit.shape_info.normal;
                            auto dir = reflect(*ray.dir, refl_normal);
                            auto shape_info_for_biasing = hit.shape_info;
                            shape_info_for_biasing.normal = refl_normal;

                            auto dir_sample = math::sample_cone(nsamp, dir, roughness(mat) * math::pi);
                            ray_type refl = biased_ray({
                                hit.shape_info.hit_point,
                                dir_sample,
                            }, shape_info_for_biasing);

                            counter.ray_refl++;
                            reflection = trace_ray(refl, remaining_bounces);
                        }
                        if (refractive(mat)) {
                            auto into = (dot(*ray.dir, *hit.shape_info.normal) < 0.0f);
                            auto ior = index_of_refraction(mat);
                            auto eta = (into ? 1.0f / ior : ior);
                            auto refr_normal = (into ? 1.0f : -1.0f) * *hit.shape_info.normal;
                            auto dir = refract(*ray.dir, refr_normal, eta);

                            if (!std::isnan(dir.x)) {   // not total internal reflection
                                auto dir_sample = math::sample_cone(nsamp, dir, roughness(mat) * math::pi);

                                auto shape_info_for_biasing = hit.shape_info;
                                shape_info_for_biasing.normal = -refr_normal;
                                ray_type refr = biased_ray({
                                    hit.shape_info.hit_point,
                                    dir_sample,
                                }, shape_info_for_biasing);

                                counter.ray_refr++;
                                refraction = trace_ray(refr, remaining_bounces);
                            }
                        }

                        return spp.push(hit, reflection, refraction);
                    }
                );
            }

            shading_point_pool spp{};
        };

        auto shade(shading_point_pool const& spp, scene_type const& scene, math::normal_sampler & samp)
        {
            std::vector<color_type> radiances;
            radiances.reserve(spp.size() + 1);
            radiances.emplace_back();    // setup guard element

            for (auto& sp: spp) {
                auto radiance = sp.hit.match(
                    [&] (hits::missed m) {
                        return shade_environment(scene, m.viewing);
                    },
                    [&] (hits::object hit) {
                        auto refl = radiances[sp.reflection];
                        auto refr = radiances[sp.refraction];
                        auto diffuse = shade_diffuse(scene, hit, samp);
                        auto radiance = shade_illumination(scene, hit, diffuse, refl, refr);
                        return radiance;
                    });
                radiances.emplace_back(radiance);
            }

            return radiances;
        }

        auto visualize(shading_point_pool const& spp)
        {
            ray_visualizations rvs;
            rvs.reserve(spp.size() * 2);

            std::vector<char> known(spp.size()+1, false);
            known[0] = true;    // setup guard element

            auto push_ray_unless_known = [&] (int sp_id, color_type color, float width) {
                if (known[sp_id]) return;
                known[sp_id] = true;
                auto& sp = spp[sp_id];
                auto viewing = viewing_ray(sp.hit);
                auto extent = ray_extent(sp.hit);
                rvs.emplace_back(viewing, extent, color, width);
            };

            push_ray_unless_known(spp.size(), color_primary_ray, width_primary_ray);
            for (auto& sp: spp) {
                sp.hit.match(
                    [&] (hits::missed) {},
                    [&] (hits::object hit) {
                        ray_type normal_ray{
                            .origin = hit.shape_info.hit_point,
                            .dir = hit.shape_info.normal,
                        };
                        rvs.emplace_back(normal_ray, length_normal_ray, color_normal_ray, width_normal_ray);
                        push_ray_unless_known(sp.reflection, color_reflective_ray, width_reflective_ray);
                        push_ray_unless_known(sp.refraction, color_refractive_ray, width_refractive_ray);
                    });
            }

            return rvs;
        }
    }

    raytracing_result_type raytrace(
            scene_type const& scene,
            view_type const& view,
            util::tile const& tile)
    {
        raytracer_impl impl{scene};
        auto& cam = view.camera;
        auto s2cp = view.screen_space_to_camera_plane_space();

        const int max_samples = view.samples > 0 ? view.samples : 1;

        image::image<std::vector<int>> shading_point_roots{{tile.w, tile.h}};
        shading_point_roots.each([&] (auto& sp_ids, auto pos) {
            for (int i=0; i<max_samples; i++) {
                counter.pixel++;
                auto screen_pos = glm::vec2{pos + glm::ivec2{tile.x, tile.y}};
                screen_pos.x += impl.nsamp();
                screen_pos.y += impl.nsamp();
                auto p = s2cp * glm::vec3{screen_pos, 1};
                auto ray = camera_ray_from_camera_plane(glm::vec2{p}, cam);
                auto sp_id = impl.trace_ray(ray, view.bounces);
                sp_ids.emplace_back(sp_id);
            }
        });

        auto radiances = shade(impl.spp, scene, impl.nsamp);

        image_type img{{tile.w, tile.h}};
        hit_buffer_type buf{{tile.w, tile.h}};
        shading_point_roots.each([&] (auto& sp_ids, auto pos) {
            buf[pos] = impl.spp[sp_ids[0]].hit;
            color_type sum{};
            for (auto sp_id: sp_ids) sum += radiances[sp_id];
            img[pos] = sum / float(sp_ids.size());
        });

        return { img, buf };
    }

    ray_visualizations raytrace(scene_type const& scene, view_type const& view, glm::vec2 screen_pos)
    {
        counter.pixel++;
        auto& cam = view.camera;
        auto s2cp = view.screen_space_to_camera_plane_space();
        auto p = s2cp * glm::vec3{screen_pos, 1.0f};
        auto ray = camera_ray_from_camera_plane(glm::vec2{p}, cam);

        raytracer_impl impl{scene};
        impl.trace_ray(ray, view.bounces);
        auto rvs = visualize(impl.spp);

        return rvs;
    }
}

