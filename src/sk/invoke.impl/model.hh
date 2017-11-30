#pragma once
// A model is an hemesh with selections
#include "../../hegem/hemesh.hh"
#include <vector>

namespace rt::sk
{
    namespace op::invoke_impl
    {
        struct model
        {
            hegem::hemesh hmesh;
            std::vector<hegem::face_type*> selected_faces;
        };
    }
}

