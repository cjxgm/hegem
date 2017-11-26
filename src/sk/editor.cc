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
            int & default_node_width,
            editor::draw_state & state)
        {
            ImGui::BeginChild("sk editor region", {}, {}, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            auto window_hovered = ImGui::IsWindowHovered();
            auto window_origin = to_glm(ImGui::GetCursorScreenPos());
            auto origin_offset = to_glm(ImGui::GetCursorPos());

            auto screen_to_grid = [=] (glm::vec2 screen) {
                return glm::ivec2{(screen - window_origin - origin) / grid_size};
            };

            auto grid_to_screen = [=] (glm::ivec2 grid) {
                return glm::vec2{grid} * grid_size + origin + window_origin;
            };

            auto grid_to_local = [=] (glm::ivec2 grid) {
                return glm::vec2{grid} * grid_size + origin + origin_offset;
            };

            { // new node placeholder button and popup menu
                if (window_hovered) {
                    auto mouse_grid = screen_to_grid(to_glm(ImGui::GetMousePos()));
                    auto placeholder_width = g.find_empty_width(mouse_grid.x, mouse_grid.y, default_node_width);

                    if (placeholder_width != 0) {
                        auto pos = grid_to_local(mouse_grid);
                        auto size = glm::vec2{float(placeholder_width), 1.0f} * grid_size;
                        ImGui::PushStyleColor(ImGuiCol_Text, ImColor{80, 80, 80});
                        ImGui::PushStyleColor(ImGuiCol_Button, ImColor{50, 50, 50});
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor{50, 50, 50});
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor{45, 45, 45});
                        ImGui::SetCursorPos(to_imgui(pos));
                        if (ImGui::Button("New node here...", to_imgui(size))) {
                            state.new_node_x = mouse_grid.x;
                            state.new_node_y = mouse_grid.y;
                            state.new_node_w = placeholder_width;
                            ImGui::OpenPopup("new node");
                        }
                        ImGui::PopStyleColor(4);
                    }
                }

                if (ImGui::BeginPopup("new node")) {
                    auto pos = grid_to_screen({ state.new_node_x, state.new_node_y });
                    ImGui::SetWindowPos(to_imgui(pos));

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
                        if (ImGui::Button(op.name)) {
                            auto& node = g.emplace(
                                state.new_node_x,
                                state.new_node_y,
                                state.new_node_w,
                                id);
                            selection = node.id;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::PopStyleColor(4);
                    }
                    ImGui::EndPopup();
                }
            }

            ImGui::EndChild();
        }
    }

    void editor::draw()
    {
        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor{40, 40, 40});
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColor{50, 50, 50});
        ImGui::PushStyleColor(ImGuiCol_Border, ImColor{30, 30, 30});
        ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImColor{0, 0, 0, 0});

        draw_editor(g, selection, grid_size, origin, default_node_width, state);

        ImGui::PopStyleColor(4);
    }
}

