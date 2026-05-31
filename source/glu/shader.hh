#pragma once
#include "../lib/gl/gl.hh"
#include "../lib/std/optional.hh"
#include "../util/as-czstring.hh"
#include "resource.hh"
#include <string>

namespace rt::glu
{
    namespace shader_factory
    {
        shared_program program_from_name(util::as_czstring name, std::string const& injection={});
    }
}

