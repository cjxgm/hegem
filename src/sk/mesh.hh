#pragma once
// Convert to triangle mesh
#include "../scene/shape/mesh.hh"
#include "invoke.impl/model.fwd.hh"

namespace rt::sk
{
    inline namespace mesh
    {
        auto build_selection_mesh(op::invoke_impl::model const& m) -> scene::shapes::mesh;
    }
}

