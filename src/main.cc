#include "lib/sanity-check.hh"
#include "app.hh"
#include "options.hh"
#include <iostream>
#include <stdexcept>

int main(int argc, char const* argv[]) try
{
    ifs::options opts{argc, argv};
    ifs::app::run(opts);
}
catch (std::runtime_error const& ex)
{
    std::cerr << ex.what() << "\n";
    return 1;
}

