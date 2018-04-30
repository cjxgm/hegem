#include "../lib/imgui.hh"
#include "progress-chooser.hh"
#include <cmath>

namespace rt::morpha
{
    auto progress_chooser(int* progress) -> bool
    {
        auto modified = false;

        ImGui::PushID("progress-chooser");
        auto area_width = ImGui::GetContentRegionAvailWidth();
        auto frame_height = ImGui::GetFrameHeight();
        auto item_spacing = ImGui::GetStyle().ItemSpacing;
        auto extreme_shortcut_width = (area_width - item_spacing.x * 2.0f) / 3.0f;
        auto extreme_shortcut_height = frame_height * 2.0f;

        if (ImGui::Button("0%", ImVec2{extreme_shortcut_width, extreme_shortcut_height})) {
            if (*progress != 0) {
                modified |= true;
                *progress = 0;
            }
        }
        ImGui::SameLine();

        if (ImGui::Button("50%", ImVec2{extreme_shortcut_width, extreme_shortcut_height})) {
            if (*progress != 50) {
                modified |= true;
                *progress = 50;
            }
        }
        ImGui::SameLine();

        if (ImGui::Button("100%", ImVec2{extreme_shortcut_width, extreme_shortcut_height})) {
            if (*progress != 100) {
                modified |= true;
                *progress = 100;
            }
        }

        ImGui::Spacing();

        ImGui::PushItemWidth(area_width);
        if (ImGui::SliderInt("###progress", progress, 0.0f, 100.0f, "%.0f%%")) {
            modified |= true;
        }

        ImGui::PopID();
        return modified;
    }
}

