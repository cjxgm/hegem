#pragma once
#include "../tool/as-czstring.hxx"
#include "scene.hxx"
#include <iostream>

namespace hegem::scene
{
    scene_type from_istream(std::istream & ist);
    scene_type from_path(tool::as_czstring path);
}

