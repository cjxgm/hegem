#pragma once
#include "../tool/constraints.hxx"
#include "op.hxx"
#include <fstream>
#include <string>

namespace hegem::skein
{
    namespace serializer
    {
        struct toml: tool::non_transferable
        {
            struct instance: tool::non_transferable
            {
                auto value(char const* var, float x) -> void;
                auto value(char const* var, int x) -> void;
                auto value(char const* var, bool x) -> void;
                auto value(char const* var, op::float3 x) -> void;

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

