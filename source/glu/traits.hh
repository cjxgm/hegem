#pragma once
#include "../lib/gl/gl.hh"
#include <type_traits>
#include <utility>      // for std::integer_sequence

namespace rt::glu
{
    namespace traits
    {
        template <gl::enum_type e>
        using gl_enum = std::integral_constant<gl::enum_type, e>;

        template <gl::enum_type ...es>
        using gl_enum_sequence = std::integer_sequence<gl::enum_type, es...>;

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

        namespace shader_details
        {
            template <gl::enum_type Type>
            struct impl;

            #define DEFINE_SHADER_TRAIT(TYPE, NAME_ABBR) \
                template<> \
                struct impl<TYPE> \
                { \
                    static constexpr auto name_abbr() { return NAME_ABBR; } \
                }
            DEFINE_SHADER_TRAIT(gl::vertex_shader,          "vert");
            DEFINE_SHADER_TRAIT(gl::tess_control_shader,    "ctrl");
            DEFINE_SHADER_TRAIT(gl::tess_evaluation_shader, "eval");
            DEFINE_SHADER_TRAIT(gl::geometry_shader,        "geom");
            DEFINE_SHADER_TRAIT(gl::fragment_shader,        "frag");
            DEFINE_SHADER_TRAIT(gl::compute_shader,         "comp");
            #undef DEFINE_SHADER_TRAIT

            template <gl::enum_type Type>
            using shader = impl<Type>;

            using shader_types = gl_enum_sequence<
                gl::vertex_shader,
                gl::tess_control_shader,
                gl::tess_evaluation_shader,
                gl::geometry_shader,
                gl::fragment_shader,
                gl::compute_shader
            >;
        }
        using shader_details::shader;
        using shader_details::shader_types;
    }
}

