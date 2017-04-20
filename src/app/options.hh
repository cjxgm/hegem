#pragma once
#include "../scene/scene.hh"
#include <string>
#include <deque>

namespace rt::app
{
    struct options
    {
        std::string scene_basedir{"scene"};
        std::deque<scene::loadable_scene> scenes;

        options(int argc, char const* argv[]);
    };
}

