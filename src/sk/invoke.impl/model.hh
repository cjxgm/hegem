#pragma once
// A model is an hemesh with selections
#include "../../hegem/hemesh.hh"
#include <unordered_set>

namespace rt::sk
{
    namespace op::invoke_impl
    {
        struct model
        {
            hegem::hemesh hmesh;
            std::unordered_set<hegem::face_type*> face_selection;
            std::unordered_set<hegem::vert_type*> vert_selection;
        };
    }
}

