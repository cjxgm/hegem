#include "../lib/imgui.hh"
#include "editor.hh"
#include "progress-chooser.hh"

namespace rt::morpha
{
    struct editor::temporary_state
    {
        int morphing_progress{};
    };

    editor::editor(): tmp{std::make_unique<temporary_state>()} {}
    editor::~editor() = default;

    void editor::draw()
    {
        if (progress_chooser(&tmp->morphing_progress)) {
            ImGui::Text("%s", "Changed");
        }
    }
}

