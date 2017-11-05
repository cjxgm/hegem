#include "hemesh.fwd.hh"
#include "serialize.hh"
#include <unordered_map>

namespace rt::hemesh
{
    struct cpp_serializer final: serializer
    {
        void declare_structure(char const* type, char const* name, void const* ptr) override;

        void begin_structure(char const* type, char const* name, void const* ptr) override;
        void end_structure() override;
        void field_ptr_from_slab(char const* type, char const* name, void const* ptr) override;
        void field(char const* type, char const* name, position_type pos) override;

    private:
        std::unordered_map<char const*, int> counters;
        std::unordered_map<void const*, int> pointer_ids;
    };

    struct dump_serializer final: serializer
    {
        void declare_structure(char const* type, char const* name, void const* ptr) override;

        void begin_structure(char const* type, char const* name, void const* ptr) override;
        void end_structure() override;
        void field_ptr_from_slab(char const* type, char const* name, void const* ptr) override;
        void field(char const* type, char const* name, position_type pos) override;

    private:
        std::unordered_map<char const*, int> counters;
        std::unordered_map<void const*, int> pointer_ids;
        std::unordered_map<char const*, char const*> type_names;
    };

    void dump(hemesh const& m, bool starts_with_newline=true);
    void dump_cpp(hemesh const& m, bool starts_with_newline=true);
    void dump_pretty(hemesh const& m, bool starts_with_newline=true);
    void dump_pointer(hemesh const& m, void const* ptr);    // this is slow
}

