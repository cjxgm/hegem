#pragma once
#include "../lib/std/any.hxx"
#include "../util/span.hxx"
#include "op.hxx"

namespace hegem::skein
{
    namespace op
    {
        auto invoke(op_instance const& instance, util::span<lib::any> arguments) -> lib::any;
    }
}

