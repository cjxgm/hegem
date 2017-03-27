#pragma once
#include <string>
#include <utility>
#include "../lib/cxxopts.hh"

namespace rt::app
{
    struct options
    {
        #define OPTION(TYPE, VAR, ...) const TYPE VAR;
        #include "options.def.inl"

        options(int argc, char const* argv[]);

        #define BEGIN_OPTIONS options(
        #define END_OPTIONS )
        #define MORE_OPTIONS ,
        #define OPTION(TYPE, VAR, ...) TYPE VAR
        #include "options.def.inl"
            #define BEGIN_OPTIONS :
            #define MORE_OPTIONS ,
            #define OPTION(TYPE, VAR, ...) VAR{std::move(VAR)}
            #include "options.def.inl"
        {
            sanity_check();
        }

    private:
        options(cxxopts::Options const& opts);
        void sanity_check() const;
    };
}

