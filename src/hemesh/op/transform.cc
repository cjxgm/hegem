#include "../../lib/glm/mat4.hh"
#include "../primitive.hh"
#include "../list.hh"
#include "transform.hh"
#include <unordered_set>

namespace rt::hemesh
{
    inline namespace op
    {
        inline namespace transform
        {
            void affine_transform_all(body_type* any_body, glm::mat4 m)
            {
                for (auto& b: list::iterate(any_body))
                    affine_transform(&b, m);
            }

            void affine_transform(body_type* b, glm::mat4 m)
            {
                // Since a vertex may be shared by multiple faces,
                // We cannot simply:
                //
                //      for (auto& f: list::iterate(b->any_face))
                //          affine_transform(&f, m);
                //
                // It will transform the same vertex multiple times.
                std::unordered_set<vert_type*> verts;
                for (auto& f: list::iterate(b->any_face))
                    for (auto& r: list::iterate(f.boundary))
                        for (auto& h: list::iterate(r.any_hege))
                            verts.emplace(h.start);

                for (auto v: verts) affine_transform(v, m);
            }

            void affine_transform(face_type* f, glm::mat4 m)
            {
                for (auto& r: list::iterate(f->boundary))
                    affine_transform(&r, m);
            }

            void affine_transform(ring_type* r, glm::mat4 m)
            {
                for (auto& h: list::iterate(r->any_hege))
                    affine_transform(h.start, m);
            }

            void affine_transform(edge_type* e, glm::mat4 m)
            {
                auto h0 = e->any_hege;
                auto h1 = h0->twin;
                affine_transform(h0->start, m);
                affine_transform(h1->start, m);
            }

            void affine_transform(vert_type* v, glm::mat4 m)
            {
                v->pos = glm::vec3{m * glm::vec4{v->pos, 1.0f}};
            }
        }
    }
}

