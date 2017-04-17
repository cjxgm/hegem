#pragma once
#include "../utils/as-czstring.hh"
#include "../utils/constraints.hh"

namespace rt::gui
{
    namespace wsi
    {
        // NOTE: there can only be 1 instance in the whole application lifetime
        struct context : utils::non_transferable
        {
            context(utils::as_czstring title);
            ~context();

            void mainloop();
        };
    }
}

