#pragma once
#include "../lib/gl/gl.hh"
#include <type_traits>

namespace rt::gui
{
    namespace gl_traits
    {
        template <gl::enum_type e>
        using gl_enum = std::integral_constant<gl::enum_type, e>;

        namespace equiv_unsigned_type_enum_details
        {
            template <int Size> struct impl;
            template<> struct impl<2>: gl_enum<gl::unsigned_short> {};
            template<> struct impl<4>: gl_enum<gl::unsigned_int> {};

            template <int Size>
            static constexpr auto equiv_unsigned_type_enum_by_size = impl<Size>::value;
            template <class T>
            static constexpr auto equiv_unsigned_type_enum = impl<sizeof(T)>::value;
        }
        using equiv_unsigned_type_enum_details::equiv_unsigned_type_enum_by_size;
        using equiv_unsigned_type_enum_details::equiv_unsigned_type_enum;
    }
}

