#include "../tool/file-dialog.hxx"
#include "../lib/glm/op/common.hxx"
#include "../lib/imgui.hxx"
#include "file-slot.hxx"

namespace hegem::silo
{
    struct file_slot::temporary_state
    {
        tool::file_dialog dialog;
        std::string filename;
        std::string empty_string;
    };

    file_slot::file_slot(): tmp{std::make_unique<temporary_state>()} {}
    file_slot::~file_slot() = default;

    auto file_slot::draw(char const* title, char const* directory) -> std::string const&
    {
        auto area_width = ImGui::GetContentRegionAvailWidth();
        auto btn_width = glm::min(area_width * 0.9f, 60.0f);

        ImGui::PushID(title);
        ImGui::Columns(2, title, false);
        ImGui::SetColumnWidth(0, btn_width);
        if (tmp->dialog) {
            ImGui::AlignTextToFramePadding();
            ImGui::TextDisabled("%s", "    ...");
        } else {
            if (ImGui::Button("Load")) {
                tmp->dialog.open(
                    tool::file_dialog::action::open,
                    title,
                    directory);
            }
        }
        ImGui::NextColumn();
        ImGui::AlignTextToFramePadding();
        if (tmp->filename.empty()) {
            ImGui::TextDisabled("(no file loaded as %s)", title);
        } else {
            ImGui::Text("%s", tmp->filename.data());
        }
        ImGui::Columns();
        ImGui::PopID();

        if (auto opt_filename = tmp->dialog()) {
            tmp->filename = std::move(*opt_filename);
            return tmp->filename;
        } else {
            return tmp->empty_string;
        }
    }
}

