#include "../lib/boost/format.hxx"
#include "serializer.hxx"
#include <stdexcept>
#include <ios>

namespace hegem::skein::serializer
{
    namespace
    {
        // format_float(1.234f)         => "1.234"
        // format_float(1.0f)           => "1.0"
        // format_float(0.0f)           => "0.0"
        // format_float(1.0f / 0.0f)    => "inf"
        // format_float(0.0f / 0.0f)    => "nan"
        auto format_float(float x)
        {
            return str(boost::format("%||%||") % x % (x == float(int(x)) ? ".0" : ""));
        }
    }

    toml::toml(std::string const& path)
        : ofs{path}
    {
        if (!ofs) throw std::runtime_error{"Failed to open file for write."};
    }

    auto toml::node(char const* id, int x, int y, int w, bool previewing) -> instance
    {
        ofs
            << "[[node]]\n"
            << "pos = [" << x << ", " << y << "]\n"
            << "width = " << w << "\n"
            << (previewing ? "preview = true\n" : "")
        ;
        return {ofs, id};
    }

    toml::instance::instance(std::ofstream& ofs, char const* id)
        : ofs{ofs}
    {
        ofs << "[node.instance." << id << "]\n";
    }

    toml::instance::~instance()
    {
        ofs << "\n";
    }

    auto toml::instance::value(char const* var, float x) -> void
    {
        ofs << boost::format("%|| = %||\n") % var % format_float(x);
    }

    auto toml::instance::value(char const* var, int x) -> void
    {
        ofs << boost::format("%|| = %||\n") % var % x;
    }

    auto toml::instance::value(char const* var, bool x) -> void
    {
        ofs << boost::format("%|| = %||\n") % var % boost::io::group(std::boolalpha, x);
    }

    auto toml::instance::value(char const* var, op::float3 x) -> void
    {
        ofs << boost::format("%|| = [%||, %||, %||]\n")
            % var
            % format_float(x[0])
            % format_float(x[1])
            % format_float(x[2])
        ;
    }
}

