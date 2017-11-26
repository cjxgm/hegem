#include "../lib/imgui.hh"
#include "editor.hh"

namespace rt::sk
{
    namespace
    {
        auto to_glm(ImVec2 a) { return glm::vec2{a.x, a.y}; }
        auto to_imgui(glm::vec2 a) { return ImVec2{a.x, a.y}; }
        auto to_imcolor(op::color_type a) { return ImColor{a.x, a.y, a.z}; }

        void draw_editor(
            graph & g,
            node_id_type & selection,
            glm::vec2 & grid_size,
            glm::vec2 & origin,
            int & default_node_width)
        {
            ImGui::BeginChild("sk editor region", {}, {}, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            auto window_hovered = ImGui::IsWindowHovered();
            auto window_origin = to_glm(ImGui::GetCursorScreenPos());
            auto origin_offset = to_glm(ImGui::GetCursorPos());
            auto mouse_in_window = to_glm(ImGui::GetMousePos()) - window_origin;
            auto mouse_grid = glm::ivec2{(mouse_in_window - origin) / grid_size};

            { // new node
                auto size = glm::vec2{float(default_node_width), 1.0f} * grid_size;
                auto pos = glm::vec2{mouse_grid} * grid_size + origin + origin_offset;

                if (window_hovered && g.find_node(mouse_grid.x, mouse_grid.y) == nullptr) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImColor{80, 80, 80});
                    ImGui::PushStyleColor(ImGuiCol_Button, ImColor{50, 50, 50});
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor{50, 50, 50});
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor{45, 45, 45});
                    ImGui::SetCursorPos(to_imgui(pos));
                    if (ImGui::Button("New node here...", to_imgui(size)))
                        ImGui::OpenPopup("new node");
                    ImGui::PopStyleColor(4);
                }

                ImGui::SetNextWindowPos(to_imgui(pos + window_origin), ImGuiSetCond_Appearing);
                if (ImGui::BeginPopup("new node")) {
                    kind_metadata const* prev_kind = nullptr;
                    for (auto& entry: g.op_metadata_range()) {
                        auto id = entry.first;
                        auto& op = entry.second;


                        if (op.kind != prev_kind) {
                            prev_kind = op.kind;
                            ImGui::AlignFirstTextHeightToWidgets();
                            ImGui::PushStyleColor(ImGuiCol_Text, to_imcolor(op.kind->color_fg_accent));
                            ImGui::Text(prev_kind->name);
                            ImGui::PopStyleColor(1);
                        }
                        ImGui::SameLine();

                        ImGui::PushStyleColor(ImGuiCol_Text, to_imcolor(op.kind->color_fg));
                        ImGui::PushStyleColor(ImGuiCol_Button, ImColor{0, 0, 0, 0});
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, to_imcolor(op.kind->color_bg));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, to_imcolor(op.kind->color_bg_accent));
                        ImGui::Button(op.name);
                        ImGui::PopStyleColor(4);
                    }
                    ImGui::EndPopup();
                }
            }

            ImGui::SetCursorPos(ImVec2{50, 50});
            ImGui::Text("Mouse (in window): %.1f %.1f\nMouse Grid: %d %d", mouse_in_window.x, mouse_in_window.y, mouse_grid.x, mouse_grid.y);

            ImGui::EndChild();
        }
    }

    void editor::draw()
    {
        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor{40, 40, 40});
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColor{50, 50, 50});
        ImGui::PushStyleColor(ImGuiCol_Border, ImColor{30, 30, 30});
        ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImColor{0, 0, 0, 0});

        draw_editor(g, selection, grid_size, origin, default_node_width);

        ImGui::PopStyleColor(4);
    }
}

