#pragma once
#include "../tool/constraints.hxx"
#include "../tool/desktop.hxx"
#include <memory>

namespace hegem::silo
{
    struct file_slot final: tool::constraints::only_movable
    {
        struct temporary_state;

        file_slot(tool::desktop_subsystem* desktop);
        ~file_slot(); // = default in implementation

        // If the user selected a file, returns a non-empty string of the filename.
        auto draw(char const* title, char const* directory) -> std::string const&;
        decltype(auto) operator () (char const* title, char const* directory) { return draw(title, directory); }

    private:
        std::unique_ptr<temporary_state> tmp;
    };
}

