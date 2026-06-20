#include "hemesh.fwd.hxx"
#include "serialize.hxx"
#include <unordered_map>

namespace hegem::swing
{
    struct cpp_serializer final: serializer
    {
        auto declare_structure(char const* type, char const* name, void const* ptr) -> void override;

        auto begin_structure(char const* type, char const* name, void const* ptr) -> void override;
        auto end_structure() -> void override;
        auto field_ptr_from_slab(char const* type, char const* name, void const* ptr) -> void override;
        auto field(char const* type, char const* name, position_type pos) -> void override;

    private:
        std::unordered_map<char const*, int> counters;
        std::unordered_map<void const*, int> pointer_ids;
    };

    struct dump_serializer final: serializer
    {
        auto declare_structure(char const* type, char const* name, void const* ptr) -> void override;

        auto begin_structure(char const* type, char const* name, void const* ptr) -> void override;
        auto end_structure() -> void override;
        auto field_ptr_from_slab(char const* type, char const* name, void const* ptr) -> void override;
        auto field(char const* type, char const* name, position_type pos) -> void override;

    private:
        std::unordered_map<char const*, int> counters;
        std::unordered_map<void const*, int> pointer_ids;
        std::unordered_map<char const*, char const*> type_names;
    };

    auto dump(hemesh const& m, bool starts_with_newline=true) -> void;
    auto dump_cpp(hemesh const& m, bool starts_with_newline=true) -> void;
    auto dump_pretty(hemesh const& m, bool starts_with_newline=true) -> void;
    auto dump_pointer(hemesh const& m, void const* ptr) -> void;    // this is slow
    auto dump_memory_usage_map(hemesh const& m) -> void;
}

