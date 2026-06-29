#pragma once
#include "types.hxx"
#include "channel.hxx"
#include <string>

namespace hegem::tool
{
    struct desktop_subsystem
    {
        struct internal_type;
        internal_type* maybe_internal{};
    };

    struct chosen_file_message
    {
        u64 serial;
        std::string maybe_path;
        // Empty path means that the user has canceled.
    };

    auto open_desktop_subsystem(desktop_subsystem* desktop) -> bool;  // -> failed.
    auto defer_close_desktop_subsystem(desktop_subsystem* desktop) -> void;  // After invoking the non-blocking defer_close_desktop_subsystem(desktop), you must run "while (poll_desktop_subsystem(desktop)) { /* must do nothing in the loop */ }" at a later time to reclaim subsystem resources. Otherwise, you broke the contract. Resources are released before poll_desktop_subsystem() eventually returns false.
    auto poll_desktop_subsystem(desktop_subsystem* desktop) -> bool;  // -> running.

    // Show a file dialog to users and ask them to choose a path to a single non-folder file for opening or saving.
    // Every call will have a reply with your specified serial to the channel under your specified transmitter.
    auto show_open_file_dialog(desktop_subsystem* desktop, tool::transmitter<chosen_file_message> tx, u64 serial, std::string title, std::string initial_folder = {}) -> void;
    auto show_save_file_dialog(desktop_subsystem* desktop, tool::transmitter<chosen_file_message> tx, u64 serial, std::string title, std::string initial_folder = {}) -> void;
}

