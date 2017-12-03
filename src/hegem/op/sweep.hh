#pragma once
#include "../type.hh"
#include "../hemesh.fwd.hh"
#include "../primitive.fwd.hh"

namespace rt::hegem
{
    inline namespace op
    {
        inline namespace sweep
        {
            void extrude(hemesh & m, face_type* face, offset_type offset);
        }
    }
}

