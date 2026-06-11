#pragma once
#include "../type.hxx"
#include "../hemesh.fwd.hxx"
#include "../primitive.fwd.hxx"

namespace hegem::swing
{
    inline namespace op
    {
        inline namespace sweep
        {
            void extrude(hemesh & m, face_type* face, offset_type offset);
        }
    }
}

