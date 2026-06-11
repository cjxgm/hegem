#include "../../swing/primitive.hxx"
#include "model.hxx"
#include "select.hxx"

namespace rt::sk
{
    namespace op::invoke_impl
    {
        namespace
        {
            template <class Slab, class T=typename Slab::value_type>
            void select(
                std::unordered_set<T*>& selection,
                bool exclusive,
                Slab& slab,
                bool inverse,
                util::span<T*> propose)
            {
                if (exclusive) {
                    if (inverse) {
                        std::unordered_set<T*> frees{begin(slab.frees), end(slab.frees)};
                        for (auto& node: slab.nodes)
                            if (frees.count(&node) == 0)
                                selection.emplace(&node);
                    } else {
                        selection.clear();
                    }
                }

                if (inverse) {
                    for (auto item: propose.range())
                        selection.erase(item);
                } else {
                    for (auto item: propose.range())
                        selection.emplace(item);
                }
            }
        }

        void select_faces(model& m, bool exclusive, util::span<swing::face_type*> faces, bool inverse)
        {
            select(m.face_selection, exclusive, m.hmesh.faces, inverse, faces);
        }

        void select_verts(model& m, bool exclusive, util::span<swing::vert_type*> verts, bool inverse)
        {
            select(m.vert_selection, exclusive, m.hmesh.verts, inverse, verts);
        }
    }
}

