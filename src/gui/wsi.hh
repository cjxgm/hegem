#pragma once
#include "../util/as-czstring.hh"
#include "../util/constraints.hh"

namespace rt::gui
{
    namespace wsi
    {
        // NOTE: there can only be 1 instance in the whole application lifetime
        struct context : util::non_transferable
        {
            context(util::as_czstring title);
            ~context();

            void mainloop();
        };
    }
}

