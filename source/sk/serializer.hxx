#pragma once
#include "../util/constraints.hxx"
#include "op.hxx"
#include <fstream>
#include <string>

namespace hegem::sk
{
    namespace serializer
    {
        struct toml: util::non_transferable
        {
            struct instance: util::non_transferable
            {
                void value(char const* var, float x);
                void value(char const* var, int x);
                void value(char const* var, bool x);
                void value(char const* var, op::float3 x);

                ~instance();

            private:
                std::ofstream& ofs;
                friend toml;

                instance(std::ofstream& ofs, char const* id);
            };

            toml(std::string const& path);
            auto node(char const* id, int x, int y, int w, bool previewing) -> instance;

        private:
            std::ofstream ofs;
        };
    }
}

