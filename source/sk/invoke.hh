#pragma once
#include "../lib/std/any.hh"
#include "../util/span.hh"
#include "op.hh"

namespace rt::sk
{
    namespace op
    {
        auto invoke(op_instance const& instance, util::span<lib::any> arguments) -> lib::any;
    }
}

