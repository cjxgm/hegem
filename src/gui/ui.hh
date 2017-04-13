#pragma once

namespace rt::gui
{
    namespace wsi
    {
        struct context;
    }

    // NOTE: there can only be 1 instance in the whole application lifetime
    struct ui
    {
        friend wsi::context;

        ~ui();

    private:
        ui();
        void render();
    };
}

