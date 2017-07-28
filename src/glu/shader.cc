#include "../lib/std/optional.hh"
#include "../util/slurp.hh"
#include "../util/journal.hh"
#include "shader.hh"
#include "traits.hh"

namespace rt::glu::shader_factory
{
    namespace
    {
        auto j() { return util::journal{"FACTORY"} << "\e[0;36m[SHADER]\e[0m "; }

        void check_shader_error(gl::uint_type id, util::as_czstring path)
        {
            gl::int_type ok;
            gl::get_shaderiv(id, gl::compile_status, &ok);
            if (ok) return;

            gl::int_type len;
            gl::get_shaderiv(id, gl::info_log_length, &len);

            std::string msg(len, '\0');
            gl::get_shader_info_log(id, len, nullptr, &msg[0]);

            std::string err{"Error when compiling shader "};
            err += path.data();
            err += ":\n";
            err += msg;
            throw std::runtime_error{err};
        }

        void check_program_error(gl::uint_type id, util::as_czstring name)
        {
            gl::int_type ok;
            gl::get_programiv(id, gl::link_status, &ok);
            if (ok) return;

            gl::int_type len;
            gl::get_programiv(id, gl::info_log_length, &len);

            std::string msg(len, '\0');
            gl::get_program_info_log(id, len, nullptr, &msg[0]);

            std::string err{"Error when linking program "};
            err += name.data();
            err += ":\n";
            err += msg;
            throw std::runtime_error{err};
        }

        template <gl::enum_type Type>
        auto maybe_shader_from_name(util::as_czstring name) -> lib::optional<shared_shader<Type>>
        {
            auto path = "support/shaders/" + name.to_string() + "/"
                + traits::shader<Type>::name_abbr() + ".glsl";

            if (auto maybe_src = util::maybe_slurp(path)) {
                j() << "\e[0;32m(" << name.data() << ")\e[0m compiling shader: " << path << "\n";
                auto src = std::move(maybe_src.value());
                auto s = shader_pool<Type>::instance().allocate();
                auto data = src.data();
                int size = src.size();
                gl::shader_source(s, 1, &data, &size);
                gl::compile_shader(s);
                check_shader_error(s, path);
                return s;
            } else {
                return {};
            }
        }

        template <gl::enum_type Type>
        void attach_available_shader(gl::uint_type prog, util::as_czstring name)
        {
            if (auto maybe_shader = maybe_shader_from_name<Type>(name)) {
                gl::attach_shader(prog, *maybe_shader);
            }
        }

        template <gl::enum_type ...Types>
        void attach_available_shaders(
            gl::uint_type prog,
            util::as_czstring name,
            traits::gl_enum_sequence<Types...>)
        {
            (attach_available_shader<Types>(prog, name), ...);
        }

        void attach_all_available_shaders(gl::uint_type prog, util::as_czstring name)
        {
            attach_available_shaders(prog, name, traits::shader_types{});
        }
    }

    shared_program program_from_name(util::as_czstring name)
    {
        j() << "\e[0;32m(" << name << ")\e[0m compiling program" << "\n";
        auto prog = program_pool::instance().allocate();
        try {
            attach_all_available_shaders(prog, name);
            gl::link_program(prog);
            check_program_error(prog, name);
        }
        catch (std::runtime_error const& ex) {
            std::string msg{"Error while preparing program "};
            msg += name.data();
            msg += ":\n";
            msg += ex.what();
            throw std::runtime_error(msg);
        }
        return prog;
    }
}

