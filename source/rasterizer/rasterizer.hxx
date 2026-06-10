#pragma once
#include "../scene/scene.hxx"
#include "../scene/view.hxx"
#include "state.hxx"

namespace rt::rasterizer
{
    void rasterize(state const& s, bool wireframed, float time);
}

