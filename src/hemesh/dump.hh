#include "primitive.fwd.hh"
#include "serialize.hh"

namespace rt::hemesh
{
    struct dump_serializer: serializer
    {
        void begin_structure(char const* name, void const* ptr) override;
        void end_structure() override;
        void field_ptr_from_slab(char const* name, void const* ptr) override;
        void field(char const* name, position_type pos) override;
    };

    struct cpp_serializer: serializer
    {
        void begin_structure(char const* name, void const* ptr) override;
        void end_structure() override;
        void field_ptr_from_slab(char const* name, void const* ptr) override;
        void field(char const* name, position_type pos) override;
    };

    void dump(rt::hemesh::body_type* body);
}

