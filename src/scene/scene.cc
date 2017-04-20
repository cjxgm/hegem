#include "scene.hh"
#include "parser.hh"
#include <stdexcept>

namespace rt::scene::scene_details
{
    namespace
    {
        struct load_loadable
        {
            loadable_scene& loadable;

            void operator () (scene_type const& /*scene*/) const
            {
                // do nothing intentionally
            }

            void operator () (filename_type const& path) const
            {
                loadable = scene::from_path(path);
            }
        };

        struct get_name
        {
            std::string const& operator () (scene_type const& scene) const
            {
                return scene.name;
            }

            std::string const& operator () (filename_type const& path) const
            {
                return path;
            }
        };
    }

    bool loaded(loadable_scene const& loadable)
    {
        return loadable.is<scene_type>();
    }

    scene_type& get_or_load(loadable_scene& loadable)
    {
        apply_visitor(load_loadable{loadable}, loadable);
        return loadable.get<scene_type>();
    }

    std::string const& name(loadable_scene const& loadable)
    {
        return apply_visitor(get_name{}, loadable);
    }
}

