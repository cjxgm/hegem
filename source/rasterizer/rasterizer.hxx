#pragma once
#include "../scene/scene.hxx"
#include "../scene/view.hxx"
#include "state.hxx"

namespace hegem::rasterizer
{
    auto rasterize(state const& s, bool wireframed, float time) -> void;
}

