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

    void ui::render()
    {
        static float background[] = {0.2667, 0.5333, 1.0000, 0.0000};
        gl::clear_bufferfv(gl::color, 0, background);

        static bool show_test_window = true;
        if (show_test_window) {
            ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Appearing);
            ImGui::ShowTestWindow(&show_test_window);
        }
    }
}

