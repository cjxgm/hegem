#include "../lib/gtk/application.hh"
#include "../lib/gtk/file-dialog.hh"
#include "file-dialog.hh"
#include "unreachable.macro.hh"
#include "constraints.hh"
#include <stdexcept>
#include <thread>
#include <utility>      // for std::move

namespace rt::util
{
    namespace
    {
        auto make_file_dialog(file_dialog::action act, std::string title) -> gtk::file_dialog
        {
            switch (act) {
                case file_dialog::action::open: {
                    gtk::file_dialog d{title, gtk::file_action::open};
                    d.add_button("_Open", gtk::response::accept);
                    d.add_button("_Cancel", gtk::response::cancel);
                    return d;
                }

                case file_dialog::action::save: {
                    gtk::file_dialog d{title, gtk::file_action::save};
                    d.set_do_overwrite_confirmation();
                    d.add_button("_Save", gtk::response::accept);
                    d.add_button("_Cancel", gtk::response::cancel);
                    return d;
                }

                default: RT_UNREACHABLE();
            }
        }

        struct dialog_command
        {
            file_dialog::action act;
            std::string title;
            lib::optional<std::string> directory;
            util::transmitter<std::string> result_tx;
        };

        struct dialog_system: util::non_transferable
        {
            static auto& instance()
            {
                static dialog_system ds;
                return ds;
            }

            void send(dialog_command cmd) { tx.send(std::move(cmd)); }

            ~dialog_system()
            {
                tx.send({});
                if (th.joinable()) th.join();
            }

        private:
            using message_type = lib::optional<dialog_command>;
            util::receiver<message_type> rx;
            util::transmitter<message_type> tx;
            std::thread th;

            dialog_system()
                : tx{rx.tx()}
                , th{[rx = std::move(rx)] () mutable {
                    while (auto opt_cmd = rx.recv()) {
                        auto& cmd = *opt_cmd;
                        auto app = gtk::application::create();

                        {
                            auto d = make_file_dialog(cmd.act, std::move(cmd.title));
                            d.set_modal(false);
                            if (cmd.directory) d.set_current_folder(*cmd.directory);
                            switch (d.run()) {
                                case gtk::response::accept:
                                    cmd.result_tx.send(d.get_filename());
                                    break;
                                case gtk::response::delete_event:
                                    [[fallthrough]];
                                case gtk::response::cancel:
                                    cmd.result_tx.send("");
                                    break;
                                default: RT_UNREACHABLE();
                            }
                        }

                        app->run();
                    }
                }}
            {}
        };
    }

    void file_dialog::open(action act, std::string title, lib::optional<std::string> directory)
    {
        if (opened()) {
            throw std::logic_error{
                "Do not call open() when opened() returns true."
            };
        }

        opt_path.emplace();

        dialog_system::instance().send(dialog_command{
            .act = act,
            .title = std::move(title),
            .directory = std::move(directory),
            .result_tx = rx.tx(),
        });
    }

    auto file_dialog::get() -> lib::optional<std::string>
    {
        if (auto opt_filename = rx.try_recv()) {
            if (opt_filename->empty()) opt_path = {};
            else opt_path = std::move(opt_filename);
        }

        if (opt_path && !opt_path->empty()) {
            auto result = std::move(opt_path);
            opt_path = {};
            return result;
        }
        return {};
    }
}

