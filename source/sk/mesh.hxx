#pragma once
// Convert to triangle mesh
#include "../scene/shape/mesh.hxx"
#include "invoke.impl/model.fwd.hxx"

namespace hegem::sk
{
    inline namespace mesh
    {
        auto build_selection_mesh(op::invoke_impl::model const& m) -> scene::shapes::mesh;
    }
}

