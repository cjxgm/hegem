#pragma once
#include "../lib/gl/gl.hxx"
#include "../lib/std/optional.hxx"
#include "../util/as-czstring.hxx"
#include "resource.hxx"
#include <string>

namespace rt::glu
{
    namespace shader_factory
    {
        shared_program program_from_name(util::as_czstring name, std::string const& injection={});
    }
}

