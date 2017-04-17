#pragma once
#include "../util/as-czstring.hh"
#include "scene.hh"
#include <iostream>

namespace rt::scene
{
    scene_type from_istream(std::istream & ist);
    scene_type from_path(util::as_czstring path);
}

