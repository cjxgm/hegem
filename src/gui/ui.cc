#include "../lib/gl/gl.hh"
#include "../lib/imgui.hh"
#include "../utils/journal.hh"
#include "ui.hh"
#include "wsi.hh"

using rt::utils::journal;

namespace rt::gui
{
    namespace
    {
        journal j() { return {"UI"}; }
    }

    ui::ui()
    {
        j() << "ui: (ctor)\n";
    }

    ui::~ui()
    {
        j() << "ui: (dtor)\n";
    }

    void ui::render(wsi::context& ctx)
    {
    }
}

