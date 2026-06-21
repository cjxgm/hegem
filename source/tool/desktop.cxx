#include "../lib/gtk/application.hxx"
#include "../lib/gtk/file-dialog.hxx"
#include "../lib/std/optional.hxx"
#include "desktop.hxx"
#include "unreachable.macro.hxx"
#include "constraints.hxx"
#include <thread>
#include <utility>      // for std::move

namespace hegem::tool
{
    inline namespace
    {
        struct dialog_command
        {
            enum struct action
            {
                open,
                save,
            };

            tool::transmitter<chosen_file_message> tx;
            u64 serial;
            action act;
            std::string title;
            std::string initial_folder;
        };

        auto make_file_dialog(dialog_command::action act, std::string title) -> gtk::file_dialog
        {
            switch (act) {
                case dialog_command::action::open: {
                    gtk::file_dialog d{title, gtk::file_action::open};
                    d.add_button("_Open", gtk::response::accept);
                    d.add_button("_Cancel", gtk::response::cancel);
                    return d;
                }

                case dialog_command::action::save: {
                    gtk::file_dialog d{title, gtk::file_action::save};
                    d.set_do_overwrite_confirmation();
                    d.add_button("_Save", gtk::response::accept);
                    d.add_button("_Cancel", gtk::response::cancel);
                    return d;
                }

                default: HEGEM_UNREACHABLE();
            }
        }
    }

    struct desktop_subsystem::internal_type: tool::non_transferable
    {
        using message_type = lib::optional<dialog_command>;
        tool::receiver<message_type> rx;
        tool::transmitter<message_type> tx;
        std::thread th;
        bool closing{};

        auto send(dialog_command cmd) -> void { tx.send(std::move(cmd)); }

        ~internal_type()
        {
            tx.send({});
            if (th.joinable()) th.join();
        }

        internal_type()
        : tx{rx.tx()}
        , th{[rx = std::move(rx)] () mutable { run_thread(std::move(rx)); }}
        {}

        static auto run_thread(tool::receiver<message_type> rx) -> void
        {
            while (auto opt_cmd = rx.recv()) {
                auto& cmd = *opt_cmd;
                auto app = gtk::application::create();

                {
                    auto d = make_file_dialog(cmd.act, std::move(cmd.title));
                    d.set_modal(false);
                    if (!cmd.initial_folder.empty()) d.set_current_folder(cmd.initial_folder);
                    switch (d.run()) {
                        case gtk::response::accept: {
                            cmd.tx.send(chosen_file_message{
                                .serial = cmd.serial,
                                .maybe_path = d.get_filename(),
                            });
                            break;
                        }

                        case gtk::response::delete_event: [[fallthrough]];
                        case gtk::response::cancel: {
                            cmd.tx.send(chosen_file_message{
                                .serial = cmd.serial,
                                .maybe_path = {},
                            });
                            break;
                        }

                        default: HEGEM_UNREACHABLE();
                    }
                }

                app->run();
            }
        }
    };

    auto open_desktop_subsystem(desktop_subsystem* desktop) -> bool
    {
        if (desktop->maybe_internal) {
            return true;
        } else {
            desktop->maybe_internal = new desktop_subsystem::internal_type{};
            return false;
        }
    }

    auto defer_close_desktop_subsystem(desktop_subsystem* desktop) -> void
    {
        if (auto internal = desktop->maybe_internal) {
            internal->closing = true;
        } else {
            // Silently ignore.
        }
    }

    auto poll_desktop_subsystem(desktop_subsystem* desktop) -> bool
    {
        if (auto internal = desktop->maybe_internal) {
            if (internal->closing) {
                desktop->maybe_internal = {};
                delete internal;
                return false;
            } else {
                // Nothing to do for now.
                return true;
            }
        } else {
            return false;
        }
    }

    auto show_open_file_dialog(desktop_subsystem* desktop, tool::transmitter<chosen_file_message> tx, u64 serial, std::string title, std::string initial_folder) -> void
    {
        if (auto internal = desktop->maybe_internal; internal && !internal->closing) {
            internal->send(dialog_command{
                .tx = std::move(tx),
                .serial = serial,
                .act = dialog_command::action::open,
                .title = std::move(title),
                .initial_folder = std::move(initial_folder),
            });
        } else {
            tx.send(chosen_file_message{
                .serial = serial,
                .maybe_path = {},
            });
        }
    }

    auto show_save_file_dialog(desktop_subsystem* desktop, tool::transmitter<chosen_file_message> tx, u64 serial, std::string title, std::string initial_folder) -> void
    {
        if (auto internal = desktop->maybe_internal; internal && !internal->closing) {
            internal->send(dialog_command{
                .tx = std::move(tx),
                .serial = serial,
                .act = dialog_command::action::save,
                .title = std::move(title),
                .initial_folder = std::move(initial_folder),
            });
        } else {
            tx.send(chosen_file_message{
                .serial = serial,
                .maybe_path = {},
            });
        }
    }
}

