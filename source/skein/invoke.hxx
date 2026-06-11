#pragma once
#include "../lib/std/any.hxx"
#include "../tool/span.hxx"
#include "op.hxx"

namespace hegem::skein
{
    namespace op
    {
        auto invoke(op_instance const& instance, tool::span<lib::any> arguments) -> lib::any;
    }
}

