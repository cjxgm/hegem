#include "../lib/boost/format.hh"
#include "dump.hh"
#include "list.hh"
#include "primitive.hh"
#include "hemesh.hh"
#include <iostream>
#include <unordered_map>
#include <string>
#include <utility>      // for std::move

namespace rt::hemesh
{
    void cpp_serializer::declare_structure(
        char const* type,
        char const* /*name*/,
        void const* ptr)
    {
        if (counters.find(type) == end(counters))
            counters.emplace(type, 0);
        pointer_ids[ptr] = ++counters[type];
    }

    void cpp_serializer::begin_structure(
        char const* type,
        char const* /*name*/,
        void const* ptr)
    {
        std::cerr << "new (" << pointer_ids[ptr] << ") " << type << " {\n";
    }

    void cpp_serializer::end_structure()
    {
        std::cerr << "};\n";
    }

    void cpp_serializer::field_ptr_from_slab(char const* type, char const* name, void const* ptr)
    {
        std::cerr
            << "    ." << name << " = (" << type << "*)"
            << (ptr ? pointer_ids[ptr] : 0)
            << ",\n";
    }

    void cpp_serializer::field(char const* type, char const* name, position_type pos)
    {
        std::cerr << "    ." << name << " = position_type{" << pos.x << ", " << pos.y << ", " << pos.z << "},\n";
    }
}

namespace rt::hemesh
{
    void dump_serializer::declare_structure(
        char const* type,
        char const* name,
        void const* ptr)
    {
        if (counters.find(name) == end(counters))
            counters.emplace(name, 0);
        pointer_ids[ptr] = ++counters[name];
        type_names[type] = name;
    }

    void dump_serializer::begin_structure(
        char const* /*type*/,
        char const* name,
        void const* ptr)
    {
        std::cerr << "- " << name << "." << pointer_ids[ptr] << "\n";
    }

    void dump_serializer::end_structure()
    {
    }

    void dump_serializer::field_ptr_from_slab(char const* type, char const* name, void const* ptr)
    {
        std::cerr
            << "    " << name << " " << type_names[type]
            << "." << (ptr ? pointer_ids[ptr] : 0)
            << "\n";
    }

    void dump_serializer::field(char const* type, char const* name, position_type pos)
    {
        std::cerr << "    " << name << " " << pos.x << " " << pos.y << " " << pos.z << "\n";
    }
}

namespace rt::hemesh
{
    void dump(hemesh const& m)
    {
        serialize<dump_serializer>(m);
    }

    void dump_cpp(hemesh const& m)
    {
        serialize<cpp_serializer>(m);
    }
}

namespace rt::hemesh
{
    namespace
    {
        auto build_pointer_names(hemesh const& m)
        {
            std::unordered_map<std::string, int> counters;
            std::unordered_map<void const*, std::string> pointer_names;

            auto fmt_name_2 = boost::format("%s-%02x");
            auto fmt_name_4 = boost::format("%s-%04x");
            auto fmt_name_8 = boost::format("%s-%08x");
            // fmt_name_16? No I don't think it's necessary.
            // If there are actually that large number, just let it over-align.

            #define STRUCT(TYPE, VAR) \
            { \
                std::string var{#VAR}; \
                for (auto& node: m.VAR##s.nodes) { \
                    if (counters.find(var) == end(counters)) \
                        counters.emplace(var, 0); \
                    auto id = counters[var]++; \
                    auto& fmt = ( \
                        id > 0xFFFF ? fmt_name_8 : \
                        id > 0xFF   ? fmt_name_4 : \
                        fmt_name_2); \
                    pointer_names.emplace(&node, str(fmt % #VAR % id)); \
                } \
            }
            #include "primitive.inl"

            return pointer_names;
        }
    }

    void dump_pretty(hemesh const& m)
    {
        using list::iterate;

        auto name_of =
            [ptr_names = build_pointer_names(m)]
            (void const* ptr) -> std::string {
                if (ptr) return ptr_names.at(ptr);
                return "nil";
            };

        for (auto& b: iterate(m.any_body)) {
            std::cerr << "= " << name_of(&b) << "\n";
            for (auto& f: iterate(b.any_face)) {
                std::cerr
                    << "* " << name_of(&f) << ": " << name_of(f.body)
                    << "\n";
                for (auto& r: iterate(f.boundary)) {
                    auto& pos = r.any_vert->pos;
                    std::cerr
                        << "  - " << name_of(&r) << ": "
                        << name_of(r.face) << " " << name_of(r.any_vert)
                        << " (" << pos.x << ", " << pos.y << ", " << pos.z << ")"
                        << "\n";
                    for (auto& h: iterate(r.any_hege)) {
                        auto& pos = h.start->pos;
                        std::cerr
                            << "    " << name_of(&h) << " ~ " << name_of(h.twin) << ": "
                            << name_of(h.edge) << " " << name_of(h.start)
                            << " (" << pos.x << ", " << pos.y << ", " << pos.z << ")"
                            << "\n";
                    }
                }
            }
        }
    }
}

