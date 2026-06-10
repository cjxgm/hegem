#include "lib/sanity-check.hxx"
#include "app/app.hxx"
#include <iostream>
#include <stdexcept>

int main(int argc, char const* argv[]) try
{
    rt::app::run_once({argc, argv});
}
catch (std::runtime_error const& ex)
{
    std::cerr << "\e[1;31m[ERROR] \e[0;31m" << ex.what() << "\e[0m\n";
    return 1;
}

