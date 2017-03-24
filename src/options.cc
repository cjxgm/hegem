#include "options.hh"
#include <iostream>
#include <stdexcept>

namespace ifs
{
    namespace
    {
        auto make_cxxopts(int argc, char const* argv[])
        {
            cxxopts::Options opts{argv[0], "Raytracer"};

            opts.add_options()
                #define NO_ABBR
                #define ABBR(X) #X ","
                #define NO_DEFAULT
                #define DEFAULT(X) ->default_value(X)
                #define OPTION(TYPE, VAR, SHORT, LONG, DESC, DEFAULT, ...) \
                    (SHORT #LONG, DESC, cxxopts::value<TYPE>()DEFAULT)
                #include "options.def.inl"
                ("h,help", "show this help")
            ;

            auto fake_mut_argv = const_cast<char**>(argv);
            try {
                opts.parse(argc, fake_mut_argv);
            }
            catch (cxxopts::OptionParseException const& ex) {
                std::cerr << opts.help() << '\n';
                throw std::runtime_error{ex.what()};
            }

            if (opts["help"].as<bool>()) {
                std::cerr << opts.help() << '\n';
                throw std::runtime_error{"Abort due to showing help."};
            }

            return opts;
        }
    }

    options::options(cxxopts::Options const& opts)
        : options {
            #define OPTION(TYPE, VAR, SHORT, LONG, ...) opts[#LONG].as<TYPE>(),
            #include "options.def.inl"
        }
    {}

    options::options(int argc, char const* argv[])
        : options{make_cxxopts(argc, argv)}
    {}

    void options::sanity_check() const
    {
        #define NO_CHECK(VAR, DESC)
        #define CHECK_EMPTY(VAR, DESC)  if (VAR.empty()) throw std::runtime_error{DESC " cannot be empty"};
        #define CHECK_MIN(VAR, DESC, VALUE)  if (VAR < VALUE) throw std::runtime_error{DESC " must >= " #VALUE};
        #define OPTION(TYPE, VAR, SHORT, LONG, DESC, DEFAULT, CHECKER, ARGS...) CHECKER(VAR, DESC, ## ARGS)
        #include "options.def.inl"
    }
}

