#pragma once
#include "../lib/glm/mat4.hh"
#include "../lib/std/optional.hh"
#include "../util/bvh.hh"
#include "../raytracer/face-trait.hh"
#include "shape.hh"
#include "material.hh"
#include <deque>

namespace rt::scene
{
    namespace scene_details
    {
        struct scene_type;
    }
    using scene_details::scene_type;

    namespace cache_details
    {
        using mesh_face_trait = raytracer::face_trait::mesh;
        using mesh_bvh_type = util::bvh<mesh_face_trait>;
        using optional_mesh_bvh_type = lib::optional<mesh_bvh_type>;

        struct cached_object
        {
            material_id_type material_id;
            shape_type shape;
            glm::mat4 model_to_world;
            glm::mat4 world_to_model;
            optional_mesh_bvh_type opt_bvh{};

            cached_object(material_id_type material_id, shape_type shape, glm::mat4 const& model_to_world);
            cached_object(material_id_type material_id, shape_type shape, glm::mat4 const& model_to_world, mesh_bvh_type bvh);
        };

        struct scene_cache
        {
            std::deque<cached_object> objects;
        };

        scene_cache build_scene_cache(scene_type const& scene);
    }

    using cache_details::scene_cache;
    using cache_details::mesh_bvh_type;
    using cache_details::optional_mesh_bvh_type;
    using cache_details::cached_object;
    using cache_details::build_scene_cache;
}

