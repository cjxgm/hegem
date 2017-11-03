#include <iostream>
#include "dump.hh"
#include "list.hh"
#include "primitive.hh"

namespace rt::hemesh
{
    void dump_serializer::begin_structure(char const* name, void const* ptr)
    {
        std::cerr << "struct " << ptr << " " << name << "\n";
    }

    void dump_serializer::end_structure()
    {
        std::cerr << "end\n";
    }

    void dump_serializer::field_ptr_from_slab(char const* name, void const* ptr)
    {
        std::cerr << "    field.sptr " << name << " " << ptr << "\n";
    }

    void dump_serializer::field(char const* name, position_type pos)
    {
        std::cerr << "    field.pos " << name << " " << pos.x << " " << pos.y << " " << pos.z << "\n";
    }
}

namespace rt::hemesh
{
    void cpp_serializer::begin_structure(char const* name, void const* ptr)
    {
        std::cerr << "new (" << ptr << ") " << name << "_type {\n";
    }

    void cpp_serializer::end_structure()
    {
        std::cerr << "};\n";
    }

    void cpp_serializer::field_ptr_from_slab(char const* name, void const* ptr)
    {
        std::cerr << "    ." << name << " = " << ptr << ",\n";
    }

    void cpp_serializer::field(char const* name, position_type pos)
    {
        std::cerr << "    ." << name << " = position_type{" << pos.x << ", " << pos.y << ", " << pos.z << "},\n";
    }
}

namespace rt::hemesh
{
    void dump(rt::hemesh::body_type* body)
    {
        for (auto& b: list::iterate(body)) {
            std::cerr << "# body " << &b << "\n";
            for (auto& f: list::iterate(b.any_face)) {
                std::cerr << "= face " << &f << "\n";
                for (auto& r: list::iterate(f.boundary)) {
                    std::cerr << "* ring " << &r << " hege " << r.any_hege << "\n";
                    for (auto& h: list::iterate(r.any_hege)) {
                        auto& p = h.start->pos;
                        std::cerr << "  - hege " << &h << " " << p.x << "," << p.y << "," << p.z << "\n";
                        std::cerr << "    twin " << h.twin << " around " << h.start->any_hege << "\n";
                    }
                }
            }
        }
    }
}

