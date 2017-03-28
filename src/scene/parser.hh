#pragma once
#include "../lib/gsl/string-span.hh"
#include "scene.hh"
#include <iostream>

namespace rt::scene
{
    scene_type from_istream(std::istream & ist);
    scene_type from_path(gsl::cstring_span<> path);
}

