#pragma once
#include "../../util/span.hxx"
#include "../../swing/primitive.fwd.hxx"
#include "model.fwd.hxx"

namespace rt::sk
{
    namespace op::invoke_impl
    {
        // - only select:
        //      select_*(m, true, selection, false)
        //
        // - only complementary-set select:
        //      select_*(m, true, selection, true)
        //
        // - select all:
        //      select_*(m, true, {}, true)
        //
        // - select none:
        //      select_*(m, true, {}, false)
        //
        // - select:
        //      select_*(m, false, selection, false)
        //
        // - deselect:
        //      select_*(m, false, selection, true)
        void select_faces(model& m, bool exclusive, util::span<swing::face_type*> faces, bool inverse);
        void select_verts(model& m, bool exclusive, util::span<swing::vert_type*> verts, bool inverse);

        inline void select_all(model& m)
        {
            select_faces(m, true, {}, true);
            select_verts(m, true, {}, true);
        }

        inline void select_none(model& m)
        {
            select_faces(m, true, {}, false);
            select_verts(m, true, {}, false);
        }
    }
}

