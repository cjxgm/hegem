#include "../../lib/boost/format.hh"
#include "mesh.hh"
#include <fstream>

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

        void write_obj(mesh const& m, std::ostream & out)
        {
            auto fmt_vert = boost::format("v %+.5f %+.5f %+.5f\n");
            auto fmt_normal = boost::format("vn %+.5f %+.5f %+.5f\n");
            auto fmt_face = boost::format("f %1$d//%1$d %2$d//%2$d %3$d//%3$d\n");

            for (auto& v: m.verts)
                out << fmt_vert % v.position.x % v.position.y % v.position.z;

            for (auto& v: m.verts)
                out << fmt_normal % v.normal->x % v.normal->y % v.normal->z;

            for (auto& f: m.faces) {
                auto v0 = f.vert_ids[0] + 1;
                auto v1 = f.vert_ids[1] + 1;
                auto v2 = f.vert_ids[2] + 1;
                out << fmt_face % v0 % v1 % v2;
            }
        }

        void write_obj(mesh const& m, char const* path)
        {
            std::ofstream out{path};
            write_obj(m, out);
        }
    }
}

