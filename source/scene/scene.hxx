#pragma once
#include "../lib/mapbox/variant.hxx"
#include "node.hxx"
#include "view.hxx"
#include "lamp.hxx"
#include "material.hxx"
#include "cache.hxx"
#include <deque>
#include <string>

namespace rt::scene
{
    namespace scene_details
    {
        struct scene_type
        {
            using material_container_type = std::deque<material_type>;
            using view_container_type = std::deque<view_type>;
            using lamp_container_type = std::deque<lamp_type>;

            std::string name;
            view_container_type views;
            lamp_container_type lamps;
            material_container_type materials;
            node_type root;
            material_id_type environment;
            scene_cache cache;

            void rebuild_cache();
        };

        using filename_type = std::string;
        using loadable_scene = mapbox::util::variant<filename_type, scene_type>;

        bool loaded(loadable_scene const& loadable);
        scene_type& get_or_load(loadable_scene& loadable);
        std::string const& name(loadable_scene const& loadable);
    }

    using scene_details::scene_type;
    using scene_details::loadable_scene;
    using scene_details::loaded;
    using scene_details::get_or_load;
    using scene_details::name;
}

