#pragma once
#include "../lib/std/optional.hh"
#include "as-czstring.hh"

namespace rt::util
{
    // read the whole file into a string.
    lib::optional<std::string> maybe_slurp(as_czstring path);
}

