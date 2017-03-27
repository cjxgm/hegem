#include "lib/sanity-check.hh"
#include "app/app.hh"
#include <iostream>
#include <stdexcept>

int main(int argc, char const* argv[]) try
{
    rt::app::run({argc, argv});
}
catch (std::runtime_error const& ex)
{
    std::cerr << ex.what() << "\n";
    return 1;
}

