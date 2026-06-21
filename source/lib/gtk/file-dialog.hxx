#pragma once
#include <gtkmm/filechooserdialog.h>

namespace gtk
{
    using file_dialog = Gtk::FileChooserDialog;

    namespace response
    {
        using type = Gtk::ResponseType;
        inline constexpr auto accept = Gtk::RESPONSE_ACCEPT;
        inline constexpr auto cancel = Gtk::RESPONSE_CANCEL;
        inline constexpr auto delete_event = Gtk::RESPONSE_DELETE_EVENT;
    }

    namespace file_action
    {
        using type = Gtk::FileChooserAction;
        inline constexpr auto open = Gtk::FILE_CHOOSER_ACTION_OPEN;
        inline constexpr auto save = Gtk::FILE_CHOOSER_ACTION_SAVE;
    }
}

