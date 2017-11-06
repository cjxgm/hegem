#include "mesh.hh"

namespace rt::scene
{
    namespace shapes
    {
        void extend(mesh & dst, mesh const& src)
        {
            dst.verts.reserve(dst.verts.size() + src.verts.size());
            dst.faces.reserve(src.faces.size() + dst.faces.size());

            auto offset = int(dst.verts.size());
            dst.verts.insert(end(dst.verts), begin(src.verts), end(src.verts));

            for (auto& f: src.faces) {
                dst.faces.emplace_back(
                    f.vert_ids[0] + offset,
                    f.vert_ids[1] + offset,
                    f.vert_ids[2] + offset);
            }
        }
    }
}

