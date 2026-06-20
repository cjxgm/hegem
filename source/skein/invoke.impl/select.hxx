#pragma once
#include "../../tool/span.hxx"
#include "../../swing/primitive.fwd.hxx"
#include "model.fwd.hxx"

namespace hegem::skein
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
        auto select_faces(model& m, bool exclusive, tool::span<swing::face_type*> faces, bool inverse) -> void;
        auto select_verts(model& m, bool exclusive, tool::span<swing::vert_type*> verts, bool inverse) -> void;

        inline auto select_all(model& m) -> void
        {
            select_faces(m, true, {}, true);
            select_verts(m, true, {}, true);
        }

        inline auto select_none(model& m) -> void
        {
            select_faces(m, true, {}, false);
            select_verts(m, true, {}, false);
        }
    }
}

