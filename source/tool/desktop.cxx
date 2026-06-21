#include "../lib/systemd/sd-bus.hxx"
#include "../lib/std/optional.hxx"
#include "desktop.hxx"
#include "unreachable.macro.hxx"
#include "constraints.hxx"
#include "journal.hxx"
#include "guard.hxx"
#include "types.hxx"
#include <thread>
#include <utility>      // for std::move

namespace hegem::tool
{
    inline namespace
    {
        auto j() { return hegem::tool::journal{"DESKTOP"}; }

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

        auto trim_prefix_in(std::string& result, std::string const& prefix, tool::usize prefix_shorten_size) -> bool  // -> prefix_matched.
        {
            if (result.substr(0, prefix.size()) == prefix) {
                if (prefix_shorten_size < prefix.size()) {
                    result = result.substr(prefix.size() - prefix_shorten_size, result.npos);
                } else {
                    // Nothing to trim.
                }
                return true;
            } else {
                return false;
            }
        }

        // If "a" is a hex digit of /[0-9A-Fa-f]/, the result will be the decoded nibble.
        // Otherwise, the result will be 0.
        // Post condition: assert((result[0] & 0xf0u) == 0u);
        auto decode_hex_for_nibble_into(tool::u8* result, char a) -> bool  // -> success
        {
            if (a >= '0' && a <= '9') return (result[0] = (tool::u8) (a - '0'), true);
            if (a >= 'A' && a <= 'F') return (result[0] = (tool::u8) (a - ('A' - 10)), true);
            if (a >= 'a' && a <= 'f') return (result[0] = (tool::u8) (a - ('a' - 10)), true);
            return (result[0] = 0u, false);
        }

        // On failure, the result will be empty.
        auto decode_percent_encoding_in(std::string& result) -> bool  // -> success
        {
            auto write_head = result.data();
            for (auto read_head = result.data(), last = result.data() + result.size(); read_head < last; read_head++) {
                if (read_head[0] != '%') {
                    write_head[0] = read_head[0];
                    write_head++;
                } else {
                    if (read_head + 2 < last) {
                        tool::u8 nibbles[2];
                        if (decode_hex_for_nibble_into(&nibbles[0], read_head[1]) && decode_hex_for_nibble_into(&nibbles[1], read_head[2])) {
                            write_head[0] = (char) ((nibbles[0] << 4) | nibbles[1]);
                            write_head++;
                            read_head += 2;
                        } else {
                            result.resize(0u);
                            return false;
                        }
                    } else {
                        result.resize(0u);
                        return false;
                    }
                }
            }
            result.resize((tool::usize) (write_head - result.data()));
            return true;
        }

        auto decode_uri_for_local_file_path(char const* uri) -> std::string // -> local_file_path.
        // https://datatracker.ietf.org/doc/html/rfc8089
        {
            auto result = std::string{uri};
            do {
                if (trim_prefix_in(result, "file:///", 1u)) break;  // traditional
                if (trim_prefix_in(result, "file://", 0u)) return {};  // non-local
                if (trim_prefix_in(result, "file:/", 1u)) break;  // minimal
                return {};  // Not file uri, or not encoding a Linux path.
            } while (false);

            if (decode_percent_encoding_in(result)) {
                return result;
            } else {
                j() << "invalid uri: " << uri << "\n";
                return {};
            }
        }

        auto try_interactively_choose_file(dialog_command::action act, std::string const& title, std::string const& initial_folder) -> std::string
        {
            #define REPORT_UNLESS_(SD_BUS_INVOCATION)  (([&] (char const* function) -> int {  \
                auto status = int{SD_BUS_INVOCATION};  \
                if (status < 0) j() << "Failure " << status << " in " << function << ":" << __LINE__ << ": " << #SD_BUS_INVOCATION << "\n";  \
                return status;  \
            })(__FUNCTION__))

            #define BAIL_UNLESS_(SD_BUS_INVOCATION)  do {  \
                if (REPORT_UNLESS_(SD_BUS_INVOCATION) < 0) return {};  \
            } while (false)

            auto method = ([&] () -> char const* {
                switch (act) {
                    case dialog_command::action::open: return "OpenFile";
                    case dialog_command::action::save: return "SaveFile";
                }
                HEGEM_UNREACHABLE();
            })();

            // Unique name is guaranteed to have the format ":aaa.bbb".
            auto format_sender = [] (char const* unique_name) -> std::string {
                auto result = std::string{&unique_name[1]};
                for (auto& a: result) {
                    // At the time of 2026-06-24,
                    // Even though the XDG Desktop Portal specification does not say anything about dashes,
                    // they should be replaced to underscores as object path cannot have dashes but the bus unique names could have them,
                    // even through having dashes in bus names is discouraged by the D-Bus specification.
                    if (a == '.' || a == '-') {
                        a = '_';
                    } else {
                        // Keep original character.
                    }
                }
                return result;
            };

            j() << "interactive " << method << "\n";

            auto bus = (::sd_bus*) nullptr;
            auto bus_guard = tool::defer(&::sd_bus_flush_close_unrefp, &bus);
            BAIL_UNLESS_(::sd_bus_open_user(&bus));

            auto unique_name = (char const*) nullptr;
            BAIL_UNLESS_(::sd_bus_get_unique_name(bus, &unique_name));

            auto signal_path = std::string{"/org/freedesktop/portal/desktop/request/"};
            signal_path += format_sender(unique_name);
            signal_path += "/desktop_subsystem";
            j() << "watching object: " << signal_path << "\n";
            BAIL_UNLESS_(::sd_bus_match_signal(
                bus,
                nullptr,
                "org.freedesktop.portal.Desktop",
                signal_path.data(),
                "org.freedesktop.portal.Request",
                "Response",
                nullptr,
                nullptr
            ));

            auto call = (::sd_bus_message*) nullptr;
            auto call_guard = tool::defer(&::sd_bus_message_unrefp, &call);
            BAIL_UNLESS_(::sd_bus_message_new_method_call(bus, &call, "org.freedesktop.portal.Desktop", "/org/freedesktop/portal/desktop", "org.freedesktop.portal.FileChooser", method));

            BAIL_UNLESS_(::sd_bus_message_append(call, "s", nullptr));  // parent_window: s; We want non-modal non-attached dialogs.
            BAIL_UNLESS_(::sd_bus_message_append(call, "s", title.data()));  // title: s;
            BAIL_UNLESS_(::sd_bus_message_open_container(call, 'a', "{sv}"));  // options: a{sv};
            BAIL_UNLESS_(::sd_bus_message_append(call, "{sv}", "handle_token", "s", "desktop_subsystem"));
            BAIL_UNLESS_(::sd_bus_message_append(call, "{sv}", "modal", "b", 0));  // We want non-modal non-attached dialogs.
            if (!initial_folder.empty()) {
                BAIL_UNLESS_(::sd_bus_message_open_container(call, 'e', "sv"));  // "current_folder": ay;
                BAIL_UNLESS_(::sd_bus_message_append(call, "s", "current_folder"));
                BAIL_UNLESS_(::sd_bus_message_open_container(call, 'v', "ay"));
                BAIL_UNLESS_(::sd_bus_message_append_array(call, 'y', initial_folder.data(), initial_folder.size() + 1u));  // +1u because it expects a NUL-terminated byte array, not a string.
                BAIL_UNLESS_(::sd_bus_message_close_container(call));
                BAIL_UNLESS_(::sd_bus_message_close_container(call));  // close "current_folder".
            }
            BAIL_UNLESS_(::sd_bus_message_close_container(call));  // close options.

            auto reply = (::sd_bus_message*) nullptr;
            auto reply_guard = tool::defer(&::sd_bus_message_unrefp, &reply);
            BAIL_UNLESS_(::sd_bus_call(bus, call, 0, nullptr, &reply));

            auto replied_signal_path = (char const*) nullptr;
            BAIL_UNLESS_(::sd_bus_message_read(reply, "o", &replied_signal_path));
            if (signal_path != replied_signal_path) {
                j() << "watching object: " << replied_signal_path << "\n";
                BAIL_UNLESS_(::sd_bus_match_signal(
                    bus,
                    nullptr,
                    "org.freedesktop.portal.Desktop",
                    replied_signal_path,
                    "org.freedesktop.portal.Request",
                    "Response",
                    nullptr,
                    nullptr
                ));
            }

            auto response = (::sd_bus_message*) nullptr;
            auto response_guard = tool::defer(&::sd_bus_message_unrefp, &response);
            j() << "waiting file chooser\n";
            while (true) {
                auto message = (::sd_bus_message*) nullptr;
                auto message_guard = tool::defer(&::sd_bus_message_unrefp, &message);
                auto status = REPORT_UNLESS_(::sd_bus_process(bus, &message));
                if (status < 0) return {};
                if (status > 0) {
                    if (REPORT_UNLESS_(::sd_bus_message_is_signal(message, "org.freedesktop.portal.Request", "Response")) > 0) {
                        response = message;
                        message = {};
                        break;
                    } else {
                        continue;
                    }
                } else {
                    do {
                        status = REPORT_UNLESS_(::sd_bus_wait(bus, UINT64_MAX));
                        if (status < 0) return {};
                    } while (status == 0);
                }
            }

            auto disposition = (tool::u32) 0;
            BAIL_UNLESS_(::sd_bus_message_read(response, "u", &disposition));  // response: u;
            j() << "file chooser disposition: " << (int) disposition << "\n";
            if (disposition != 0) return {};  // User canceled.

            auto uri = (char const*) nullptr;
            BAIL_UNLESS_(::sd_bus_message_enter_container(response, 'a', "{sv}"));  // results: a{sv};
            while (REPORT_UNLESS_(::sd_bus_message_at_end(response, 0)) == 0) {
                auto name = (char const*) nullptr;
                BAIL_UNLESS_(::sd_bus_message_enter_container(response, 'e', "sv"));
                BAIL_UNLESS_(::sd_bus_message_read(response, "s", &name));
                if (name == std::string{"uris"}) {
                    BAIL_UNLESS_(::sd_bus_message_enter_container(response, 'v', "as"));
                    BAIL_UNLESS_(::sd_bus_message_enter_container(response, 'a', "s"));
                    BAIL_UNLESS_(::sd_bus_message_read(response, "s", &uri));
                    BAIL_UNLESS_(::sd_bus_message_exit_container(response));
                    BAIL_UNLESS_(::sd_bus_message_exit_container(response));
                } else {
                    BAIL_UNLESS_(::sd_bus_message_skip(response, "v"));
                }
                BAIL_UNLESS_(::sd_bus_message_exit_container(response));
            }
            BAIL_UNLESS_(::sd_bus_message_exit_container(response));  // close results.

            j() << "selected uri: " << uri << "\n";
            auto path = decode_uri_for_local_file_path(uri);
            return path;

            #undef BAIL_UNLESS_
            #undef REPORT_UNLESS_
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
            while (auto cmd = rx.recv()) {
                auto maybe_path = try_interactively_choose_file(cmd->act, cmd->title, cmd->initial_folder);
                if (auto& path = maybe_path; !path.empty()) {
                    j() << "chose file: " << path << "\n";
                } else {
                    j() << "canceled choosing a file\n";
                }
                cmd->tx.send(chosen_file_message{
                    .serial = cmd->serial,
                    .maybe_path = std::move(maybe_path),
                });
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

