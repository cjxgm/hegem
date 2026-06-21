#include "../../swing/primitive.hxx"
#include "model.hxx"
#include "select.hxx"

namespace hegem::skein
{
    namespace op::invoke_impl
    {
        inline namespace
        {
            template <class Slab, class T=typename Slab::value_type>
            auto select(
                std::unordered_set<T*>& selection,
                bool exclusive,
                Slab& slab,
                bool inverse,
                tool::span<T*> propose
            ) -> void
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

        auto select_faces(model& m, bool exclusive, tool::span<swing::face_type*> faces, bool inverse) -> void
        {
            select(m.face_selection, exclusive, m.hmesh.faces, inverse, faces);
        }

        auto select_verts(model& m, bool exclusive, tool::span<swing::vert_type*> verts, bool inverse) -> void
        {
            select(m.vert_selection, exclusive, m.hmesh.verts, inverse, verts);
        }
    }
}

