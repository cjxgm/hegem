#pragma once
#include "../../lib/glm/fwd.hxx"
#include "../primitive.fwd.hxx"

namespace rt::hegem
{
    inline namespace op
    {
        inline namespace transform
        {
            void affine_transform_all(body_type* any_body, glm::mat4 m);
            void affine_transform(body_type* b, glm::mat4 m);
            void affine_transform(face_type* f, glm::mat4 m);
            void affine_transform(ring_type* r, glm::mat4 m);
            void affine_transform(edge_type* e, glm::mat4 m);
            void affine_transform(vert_type* v, glm::mat4 m);
        }
    }
}

