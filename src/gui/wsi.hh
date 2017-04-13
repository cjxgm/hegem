#pragma once
#include "../lib/gsl/string-span.hh"
#include "../utils/constraints.hh"

namespace rt::gui
{
    namespace wsi
    {
        // NOTE: there can only be 1 instance in the whole application lifetime
        struct context : utils::non_transferable
        {
            context(gsl::cstring_span<> title);
            ~context();

            void mainloop();
        };
    }
}

