#include "scene-instance.hh"

namespace rt::app
{
    scene::scene_type& scene_instance()
    {
        static scene::scene_type instance;
        return instance;
    }
}

