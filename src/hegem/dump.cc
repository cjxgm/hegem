#include "../lib/boost/format.hh"
#include "dump.hh"
#include "list.hh"
#include "hemesh.hh"
#include "type.hh"
#include "meta.hh"
#include <iostream>
#include <unordered_map>
#include <string>
#include <utility>      // for std::move

namespace rt::hegem
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

namespace rt::hegem
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

namespace rt::hegem
{
    void dump(hemesh const& m, bool starts_with_newline)
    {
        if (starts_with_newline) std::cerr << "\n";
        serialize<dump_serializer>(m);
    }

    void dump_cpp(hemesh const& m, bool starts_with_newline)
    {
        if (starts_with_newline) std::cerr << "\n";
        serialize<cpp_serializer>(m);
    }
}

namespace rt::hegem
{
    std::string pointer_name(
        pointer_name_map_type const& names,
        void const* ptr)
    {
        if (ptr == nullptr) return "nil";

        auto it = names.find(ptr);
        if (it == end(names)) {
            auto id = int(usize(ptr) & 0xFFFF);
            auto fmt = boost::format("unknown-%04x");
            return str(fmt % id);
        }

        return it->second;
    }

    void dump_pretty(hemesh const& m, bool starts_with_newline)
    {
        using list::iterate;

        auto name_of =
            [ptr_names = build_pointer_name_map(m)]
            (void const* ptr) {
                return pointer_name(ptr_names, ptr);
            };

        if (starts_with_newline) std::cerr << "\n";

        if (m.any_body == nullptr) {
            std::cerr << "= nil\n";
            return;
        }

        auto fmt_point = boost::format("(%+.3f, %+.3f, %+.3f)");

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
                        << " " << fmt_point % pos.x % pos.y % pos.z
                        << "\n";
                    for (auto& h: iterate(r.any_hege)) {
                        auto& pos = h.start->pos;
                        std::cerr
                            << "    " << name_of(&h) << " ~ " << name_of(h.twin) << ": "
                            << name_of(h.edge) << " " << name_of(h.start)
                            << " " << fmt_point % pos.x % pos.y % pos.z
                            << "\n";
                    }
                }
            }
        }
    }

    void dump_pointer(hemesh const& m, void const* ptr)
    {
        std::cerr << pointer_name(build_pointer_name_map(m), ptr) << "\n";
    }
}

namespace rt::hegem
{
    void dump_memory_usage_map(hemesh const& m)
    {
        constexpr auto line_width = 76;
        auto frees = build_free_pointer_set(m);

        #define STRUCT(NAME, VAR) \
        { \
            int x = 0; \
            for (auto& node: m.VAR##s.nodes) { \
                if (x++ % line_width == 0) std::cerr << (x == 1 ? "" : "\e[1;30m") << #VAR " \e[0m"; \
                std::cerr << (frees.find(&node) == end(frees) ? "\e[1;30m.\e[0m" : "\e[1;32m#\e[0m"); \
                if (x % line_width == 0) std::cerr << "\n"; \
            } \
            if (x % line_width != 0) std::cerr << "\n"; \
        }
        #include "primitive.inl"
    }
}

