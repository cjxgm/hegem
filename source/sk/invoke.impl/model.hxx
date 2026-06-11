#pragma once
// A model is an hemesh with selections
#include "../../swing/hemesh.hxx"
#include <unordered_set>

namespace rt::sk
{
    namespace op::invoke_impl
    {
        struct model
        {
            model() = default;
            model(model &&) = default;
            model& operator = (model &&) = default;
            model(model const& m);
            model& operator = (model const& m)
            {
                using std::swap;
                model x{m};
                swap(*this, x);
                return *this;
            }

            swing::hemesh hmesh;
            std::unordered_set<swing::face_type*> face_selection;
            std::unordered_set<swing::vert_type*> vert_selection;
        };
    }
}

