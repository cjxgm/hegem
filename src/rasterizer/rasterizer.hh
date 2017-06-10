#pragma once
#include "../scene/scene.hh"
#include "../scene/view.hh"
#include "state.hh"

namespace rt::rasterizer
{
    void rasterize(state const& s, bool wireframed);
}

