#include "slurp.hxx"
#include <fstream>
#include <sstream>

namespace hegem::util
{
    lib::optional<std::string> maybe_slurp(as_czstring path)
    {
        if (std::ifstream ifs{path.data()}) {
            std::stringstream ss;
            ss << ifs.rdbuf();
            return ss.str();
        } else {
            return {};
        }
    }
}

