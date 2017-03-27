#pragma once
#include "../lib/gsl/string-span.hh"

#define WANT_VEC3
#include "../lib/glm.hh"
#undef WANT_VEC3

namespace rt::image
{
    namespace color
    {
        using linear_rgb = glm::vec3;
        using srgb = glm::tvec3<unsigned char, glm::defaultp>;

        srgb to_srgb(linear_rgb const& src);
        srgb to_srgb(gsl::cstring_span<> hex_rrggbb);
        linear_rgb to_linear_rgb(srgb const& src);
    }
}

