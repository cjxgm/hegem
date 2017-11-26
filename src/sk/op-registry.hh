#pragma once
#include "../lib/glm/vec3.hh"
#include "../util/constraints.hh"
#include "../image/color.hh"
#include "op.hh"
#include <map>

namespace rt::sk
{
    namespace op
    {
        using color_type = image::color::linear_rgb;

        struct kind_metadata
        {
            char const* id;
            char const* name;
            char const* tooltip;

            // palette
            color_type color;
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

