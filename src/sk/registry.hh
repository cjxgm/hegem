#pragma once
#include "../util/constraints.hh"
#include "op.hh"
#include "palette.hh"
#include <map>

namespace rt::sk
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

