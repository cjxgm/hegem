#include "model.hxx"
#include <unordered_map>

namespace rt::sk
{
    namespace op::invoke_impl
    {
        namespace
        {
            template <class Slab, class T=typename Slab::value_type>
            auto migrate_pointers(Slab const& old_slab, Slab& new_slab, std::unordered_set<T*> const& ptrs)
            {
                std::unordered_set<T const*> frees;
                for (auto free: old_slab.frees) frees.emplace(free);

                std::unordered_map<T const*, T*> old_to_new;
                auto new_it = begin(new_slab.nodes);
                for (auto& old: old_slab.nodes) {
                    if (frees.count(&old)) continue;
                    old_to_new.emplace(&old, &*new_it++);
                }

                std::unordered_set<T*> result;
                for (auto ptr: ptrs)
                    result.emplace(old_to_new.at(ptr));

                return result;
            }
        }

        model::model(model const& m)
            : hmesh{m.hmesh}
            , face_selection{migrate_pointers(m.hmesh.faces, hmesh.faces, m.face_selection)}
            , vert_selection{migrate_pointers(m.hmesh.verts, hmesh.verts, m.vert_selection)}
        {}
    }
}

