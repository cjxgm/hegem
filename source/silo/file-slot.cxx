#include "../lib/glm/op/common.hxx"
#include "../lib/imgui.hxx"
#include "file-slot.hxx"

namespace hegem::silo
{
    struct file_slot::temporary_state
    {
        tool::desktop_subsystem* desktop{};
        tool::receiver<tool::chosen_file_message> file_dialog_rx{};
        std::string filename{};
        std::string empty_string{};
        bool showing_file_dialog{};

        temporary_state(tool::desktop_subsystem* desktop): desktop{desktop} {}
    };

    file_slot::file_slot(tool::desktop_subsystem* desktop): tmp{std::make_unique<temporary_state>(desktop)} {}
    file_slot::~file_slot() = default;

    auto file_slot::draw(char const* title, char const* directory) -> std::string const&
    {
        auto area_width = ImGui::GetContentRegionAvailWidth();
        auto btn_width = glm::min(area_width * 0.9f, 60.0f);

        ImGui::PushID(title);
        ImGui::Columns(2, title, false);
        ImGui::SetColumnWidth(0, btn_width);
        if (tmp->showing_file_dialog) {
            ImGui::AlignTextToFramePadding();
            ImGui::TextDisabled("%s", "    ...");
        } else {
            if (ImGui::Button("Load")) {
                tmp->showing_file_dialog = true;
                tool::show_open_file_dialog(tmp->desktop, tmp->file_dialog_rx.tx(), 0u, title, directory);
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

        if (auto filename = tmp->file_dialog_rx.try_recv()) {
            tmp->showing_file_dialog = false;
            tmp->filename = std::move(filename->maybe_path);
            return tmp->filename;
        } else {
            return tmp->empty_string;
        }
    }
}

