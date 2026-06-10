#pragma once
#include "../lib/std/optional.hxx"
#include "channel.hxx"
#include <string>

namespace rt::util
{
    // - You should constantly call get() to make progress.
    // - All functions are guaranteed to not block.
    // - open() can only be called when opened() returns false.
    // - opened() returns true if called immediately after a call to open().
    // - opened() returns false if called immediately after getting a string result from calling get().
    // - opened() return value will be the same as previous call if called immediately after getting an empty result from calling get().
    // - get() never returns std::string{""}.
    struct file_dialog
    {
        enum struct action
        {
            open,
            save,
        };

        void open(action act, std::string title, lib::optional<std::string> directory={});
        auto opened() const -> bool { return opt_path.has_value(); }
        auto get() -> lib::optional<std::string>;

        operator bool () const { return opened(); }
        auto operator () () { return get(); }

    private:
        lib::optional<std::string> opt_path;
        receiver<std::string> rx;
    };
}

