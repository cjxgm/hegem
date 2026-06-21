#pragma once
#include "../lib/gl/gl.hxx"
#include "../lib/std/optional.hxx"
#include "../tool/as-czstring.hxx"
#include "resource.hxx"
#include <string>

namespace hegem::glu
{
    namespace shader_factory
    {
        shared_program program_from_name(tool::as_czstring name, std::string const& injection={});
    }
}

