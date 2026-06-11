#pragma once
#include "../util/constraints.hxx"
#include "op.hxx"
#include "palette.hxx"
#include <map>

namespace hegem::sk
{
    namespace op
    {
        struct kind_metadata
        {
            char const* id;
            char const* name;
            char const* tooltip;
            palette_type palette;
        };

        struct op_metadata
        {
            kind_metadata const* kind;
            char const* id;
            char const* name;
            char const* tooltip;
            int arity;
        };

        struct op_registry: util::constraints::only_movable
        {
            std::map<kind_id, kind_metadata> kinds;
            std::map<op_id, op_metadata> ops;

            op_registry();
        };
    }

    using op::kind_metadata;
    using op::op_metadata;
    using op::op_registry;
}

