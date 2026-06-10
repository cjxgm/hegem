#pragma once
#include "../lib/std/optional.hxx"
#include "as-czstring.hxx"

namespace rt::util
{
    // read the whole file into a string.
    lib::optional<std::string> maybe_slurp(as_czstring path);
}

