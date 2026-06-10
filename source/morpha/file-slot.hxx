#pragma once
#include "../util/constraints.hxx"
#include <memory>

namespace rt::morpha
{
    struct file_slot final: util::constraints::only_movable
    {
        struct temporary_state;

        file_slot();
        ~file_slot(); // = default in implementation

        // If the user selected a file, returns a non-empty string of the filename.
        auto draw(char const* title, char const* directory) -> std::string const&;
        decltype(auto) operator () (char const* title, char const* directory) { return draw(title, directory); }

    private:
        std::unique_ptr<temporary_state> tmp;
    };
}

