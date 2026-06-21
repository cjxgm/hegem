#pragma once
#include "../scene/scene.hxx"
#include <string>
#include <deque>

namespace hegem::app
{
    struct options
    {
        std::string scene_basedir{"scene"};
        std::deque<scene::loadable_scene> scenes;

        options(int argc, char const* argv[]);
    };
}

