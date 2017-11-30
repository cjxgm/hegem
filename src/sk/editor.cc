#include "../lib/glm/op/common.hh"
#include "../lib/imgui.hh"
#include "editor.hh"
#include "palette.hh"
#include "engine.hh"
#include <algorithm>
#include <string>

namespace rt::sk
{
    namespace editor_details
    {
        struct temporary_state
        {
            glm::ivec2 node_pos;
            int node_width;
            std::string error_message;
            palette_type error_palette{0.96f};
        };

        namespace
        {
            auto to_glm(ImVec2 a) { return glm::vec2{a.x, a.y}; }
            auto to_imgui(glm::vec2 a) { return ImVec2{a.x, a.y}; }
            auto to_imcolor(palette_color_type a) { return ImColor{a.x, a.y, a.z}; }

            auto scaling_factor(float & scaling_level) -> float
            {
                static constexpr float scalers[] = {
                    1.0000f, 1.0000f,
                    1.0000f, 1.3333f, 1.5000f, 1.6667f,
                    2.0000f, 2.5000f,
                    3.0000f,
                    4.0000f,
                    6.0000f,
                    8.0000f,
                };
                static constexpr auto scaler_count = float(sizeof(scalers) / sizeof(scalers[0]));
                static constexpr auto scaler_upperbound = scaler_count - 1.0f;

                scaling_level = std::min(std::max(scaling_level, -scaler_upperbound), scaler_upperbound);
                auto selector = int(std::abs(scaling_level));
                auto scaler = scalers[selector];
                if (scaling_level < 0.0f) scaler = 1.0f / scaler;
                return scaler;
            }

            namespace draw_edit
            {
                bool number(char const* label, float & x)
                {
                    return ImGui::DragFloat(label, &x);
                }

                bool number(char const* label, int & x)
                {
                    return ImGui::DragInt(label, &x);
                }
            }

            bool draw_fields(node & n, char const*& tooltip)
            {
                auto changed = false;
                auto& inst = n.instance;
                auto& op = *n.metadata;
                auto& kind = *op.kind;

                {
                    auto cpos = ImGui::GetCursorPos();
                    ImGui::InvisibleButton("extend width", ImVec2{400.0f, 0.0f});
                    ImGui::SetCursorPos(cpos);
                }

                if (ImGui::Button("DELETE")) n.is_garbage = true;
                ImGui::SameLine();
                ImGui::Text("%s (%s)", op.name, kind.name);
                if (ImGui::IsItemHovered()) tooltip = op.tooltip;

                ImGui::PushID("fields");
                switch (inst.id) {
                    #define OP(KIND, ID, ARITY, NAME, TOOLTIP, FIELDS...) \
                        case op_id::KIND##_##ID: { \
                            auto& fields = inst.fields.KIND##_##ID; \
                            (void) fields; \
                            FIELDS \
                        } break;
                    #define FIELD(TYPE, VAR, INITIAL, EDITING_WIDGET, NAME, TOOLTIP) \
                        ImGui::PushItemWidth(-100); \
                        ImGui::PushID(#VAR); \
                        changed |= draw_edit::EDITING_WIDGET(NAME, fields.VAR); \
                        if (ImGui::IsItemHovered()) tooltip = TOOLTIP; \
                        ImGui::PopID();
                    #include "op.inl"
                }
                ImGui::PopID();

                return changed;
            }

            bool draw_editor(
                graph & g,
                node_id_type & previewing_node,
                float & scaling_level,
                glm::vec2 & initial_grid_size,
                glm::vec2 & origin,
                int & default_node_width,
                temporary_state & tmp)
            {
                auto changed = false;
                auto& io = ImGui::GetIO();
                ImGui::BeginChild("sk editor region", {}, {}, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

                auto window_origin = to_glm(ImGui::GetCursorScreenPos());
                auto origin_offset = to_glm(ImGui::GetCursorPos());
                auto mouse_screen_pos = to_glm(ImGui::GetMousePos());
                auto scaling = scaling_factor(scaling_level);

                if (ImGui::IsWindowHovered() && io.MouseWheel != 0.0f) {
                    auto old_scaling = scaling;
                    scaling = scaling_factor(scaling_level += io.MouseWheel);
                    if (scaling != old_scaling) {
                        auto mouse_local_pos = mouse_screen_pos - window_origin;
                        origin = (origin - mouse_local_pos) * (scaling / old_scaling) + mouse_local_pos;
                    }
                }

                auto font_scaling = scaling < 1.0f
                    ? scaling * 0.5f + 0.5f
                    : std::min(2.0f, scaling * 0.4f + 0.6f);
                auto grid_size = round(initial_grid_size * scaling);
                ImGui::SetWindowFontScale(font_scaling);

                auto local_to_screen = [=] (glm::vec2 local) {
                    return local + window_origin;
                };

                auto screen_to_grid = [=] (glm::vec2 screen) {
                    return glm::ivec2{floor((screen - window_origin - origin) / grid_size)};
                };

                auto grid_to_screen = [=] (glm::ivec2 grid) {
                    return glm::vec2{grid} * grid_size + origin + window_origin;
                };

                auto grid_to_local = [=] (glm::ivec2 grid) {
                    return glm::vec2{grid} * grid_size + origin + origin_offset;
                };

                char const* tooltip{};
                palette_type const* tooltip_palette{};

                { // new node placeholder button and popup menu
                    if (ImGui::IsWindowHovered() && !ImGui::IsMouseDragging(0)) {
                        auto mouse_grid = screen_to_grid(mouse_screen_pos);
                        auto placeholder_width = g.find_empty_width(mouse_grid.x, mouse_grid.y, default_node_width);

                        if (placeholder_width != 0) {
                            auto pos = grid_to_local(mouse_grid);
                            auto size = glm::vec2{float(placeholder_width), 1.0f} * grid_size;
                            ImGui::PushStyleColor(ImGuiCol_Text, ImColor{80, 80, 80});
                            ImGui::PushStyleColor(ImGuiCol_Button, ImColor{50, 50, 50});
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor{50, 50, 50});
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor{45, 45, 45});
                            ImGui::SetCursorPos(to_imgui(pos));
                            if (ImGui::Button("+", to_imgui(size))) {
                                tmp.node_pos = mouse_grid;
                                tmp.node_width = placeholder_width;
                                ImGui::OpenPopup("new node");
                            }
                            ImGui::PopStyleColor(4);
                        }
                    }

                    if (ImGui::BeginPopup("new node")) {
                        auto pos = grid_to_screen(tmp.node_pos);
                        ImGui::SetWindowPos(to_imgui(pos));

                        kind_metadata const* prev_kind = nullptr;
                        for (auto& entry: g.op_metadata_range()) {
                            auto id = entry.first;
                            auto& op = entry.second;
                            auto& kind = *op.kind;
                            auto& palette = kind.palette;

                            ImGui::PushID(int(id));

                            if (prev_kind != &kind) {
                                prev_kind = &kind;
                                ImGui::AlignFirstTextHeightToWidgets();
                                ImGui::PushStyleColor(ImGuiCol_Text, to_imcolor(palette.fg_accent));
                                ImGui::Text(kind.name);
                                if (ImGui::IsItemHovered()) {
                                    tooltip = kind.tooltip;
                                    tooltip_palette = &palette;
                                }
                                ImGui::PopStyleColor(1);
                            }
                            ImGui::SameLine();

                            ImGui::PushStyleColor(ImGuiCol_Text, to_imcolor(palette.fg));
                            ImGui::PushStyleColor(ImGuiCol_Button, ImColor{0, 0, 0, 0});
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, to_imcolor(palette.bg));
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, to_imcolor(palette.bg_accent));
                            if (ImGui::Button(op.name)) {
                                g.emplace(
                                    tmp.node_pos.x,
                                    tmp.node_pos.y,
                                    tmp.node_width,
                                    id);
                                ImGui::CloseCurrentPopup();
                                changed |= true;
                            }
                            if (ImGui::IsItemHovered()) {
                                tooltip = op.tooltip;
                                tooltip_palette = &palette;
                            }
                            ImGui::PopStyleColor(4);

                            ImGui::PopID();
                        }

                        ImGui::EndPopup();
                    }
                }

                { // nodes
                    ImGui::PushID("nodes");
                    for (auto& node: g.node_range()) {
                        auto& op = *node.metadata;
                        auto& kind = *op.kind;
                        auto& palette = kind.palette;

                        ImGui::PushID(node.id);
                        auto node_new_x = node.x;
                        auto node_new_y = node.y;
                        auto node_new_w = node.width;

                        { // operator node button
                            auto pos = grid_to_local({ node.x, node.y });
                            auto size = glm::vec2{float(node.width) - 0.3f, 1.0f} * grid_size;

                            std::string error_message;
                            if (!node.sanity_error.empty())
                                error_message += node.sanity_error;
                            if (!node.runtime_error.empty()) {
                                if (!error_message.empty()) error_message += "\n";
                                error_message += node.runtime_error;
                            }

                            if (node.id == previewing_node) {
                                ImGui::PushStyleColor(ImGuiCol_Text, to_imcolor(palette.bg));
                                ImGui::PushStyleColor(ImGuiCol_Button, to_imcolor(palette.fg));
                                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, to_imcolor(palette.bg_accent));
                                ImGui::PushStyleColor(ImGuiCol_ButtonActive, to_imcolor(palette.fg_accent));
                            } else {
                                ImGui::PushStyleColor(ImGuiCol_Text, to_imcolor(palette.fg));
                                ImGui::PushStyleColor(ImGuiCol_Button, to_imcolor(palette.bg));
                                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, to_imcolor(palette.bg_accent));
                                ImGui::PushStyleColor(ImGuiCol_ButtonActive, to_imcolor(palette.fg_accent));
                            }

                            ImGui::PushID("operator");
                            ImGui::SetCursorPos(to_imgui(pos));
                            ImGui::Button(op.name, to_imgui(size));
                            if (!error_message.empty()) {
                                if (ImGui::IsItemHovered()) {
                                    tmp.error_message = std::move(error_message);
                                    tooltip = tmp.error_message.data();
                                    tooltip_palette = &tmp.error_palette;
                                }

                                auto base = local_to_screen(pos);
                                auto p0 = base + 0.1f * grid_size;
                                auto p1 = base + 0.3f * grid_size;
                                auto color = (node.id == previewing_node ? tmp.error_palette.fg_accent : tmp.error_palette.fg);
                                auto& draw_list = *ImGui::GetWindowDrawList();
                                draw_list.AddRectFilled(to_imgui(p0), to_imgui(p1), to_imcolor(color));
                            }
                            if (ImGui::IsItemActive()) {
                                if (ImGui::IsMouseDoubleClicked(0)) {
                                    previewing_node = node.id;
                                    changed |= true;
                                }

                                else if (ImGui::IsMouseClicked(0)) {
                                    tmp.node_pos = { node.x, node.y };
                                    tmp.node_width = node.width;
                                }

                                else if (ImGui::IsMouseDragging(0)) {
                                    auto old_mouse = to_glm(io.MouseClickedPos[0]);
                                    auto grid_delta = screen_to_grid(mouse_screen_pos) - screen_to_grid(old_mouse);

                                    auto grid = tmp.node_pos + grid_delta;
                                    auto avail_x = g.find_empty_x(grid.x, grid.y, node.id);
                                    auto width = std::min(grid.x + tmp.node_width - avail_x, tmp.node_width);
                                    if (width < 1) width = tmp.node_width;
                                    grid.x = avail_x;
                                    width = g.find_empty_width(grid.x, grid.y, width, node.id);

                                    node_new_x = grid.x;
                                    node_new_y = grid.y;
                                    node_new_w = width;
                                }
                            }

                            ImGui::PopID();
                            ImGui::PopStyleColor(4);

                            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
                                ImGui::OpenPopup("fields");
                            }

                            ImGui::PushStyleColor(ImGuiCol_Text, to_imcolor(palette.fg));
                            ImGui::PushStyleColor(ImGuiCol_Border, to_imcolor(palette.fg));
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, to_imcolor(palette.bg));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, to_imcolor(palette.fg_accent));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, to_imcolor(palette.bg_accent));
                            ImGui::PushStyleColor(ImGuiCol_Button, to_imcolor(palette.bg));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, to_imcolor(palette.fg_accent));
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, to_imcolor(palette.bg_accent));
                            ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, to_imcolor(palette.bg_accent));
                            if (ImGui::BeginPopup("fields")) {
                                auto pos = grid_to_screen({ node.x, node.y });
                                ImGui::SetWindowPos(to_imgui(pos));
                                tooltip_palette = &palette;
                                changed |= draw_fields(node, tooltip);
                                if (node.is_garbage) {
                                    if (previewing_node == node.id)
                                        previewing_node = 0;
                                    ImGui::CloseCurrentPopup();
                                    changed |= true;
                                }
                                ImGui::EndPopup();
                            }
                            ImGui::PopStyleColor(9);
                        }

                        { // node resize button
                            auto pos = grid_to_local({ node.x + node.width, node.y });
                            auto size = glm::vec2{0.3f, 1.0f} * grid_size;
                            pos.x -= 0.3f * grid_size.x;

                            ImGui::PushStyleColor(ImGuiCol_Button, to_imcolor(palette.fg_accent));
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, to_imcolor(palette.fg));
                            ImGui::PushStyleColor(ImGuiCol_ButtonActive, to_imcolor(palette.bg_accent));

                            ImGui::SetCursorPos(to_imgui(pos));
                            ImGui::Button("##resize", to_imgui(size));
                            if (ImGui::IsItemActive()) {
                                if (ImGui::IsMouseClicked(0)) {
                                    tmp.node_pos = { node.x, node.y };
                                    tmp.node_width = node.width;
                                }

                                else if (ImGui::IsMouseDragging(0)) {
                                    auto old_mouse = to_glm(io.MouseClickedPos[0]);
                                    auto grid_delta = screen_to_grid(mouse_screen_pos) - screen_to_grid(old_mouse);

                                    auto width = std::max(1, tmp.node_width + grid_delta.x);
                                    width = g.find_empty_width(tmp.node_pos.x, tmp.node_pos.y, width, node.id);
                                    node_new_w = width;
                                }
                            }

                            ImGui::PopStyleColor(3);
                        }

                        changed |= (node.x != node_new_x);
                        changed |= (node.y != node_new_y);
                        changed |= (node.width != node_new_w);
                        node.x = node_new_x;
                        node.y = node_new_y;
                        node.width = node_new_w;

                        ImGui::PopID();
                    }
                    ImGui::PopID();
                }

                if (tooltip && tooltip[0] != '\0') {
                    ImGui::PushStyleColor(ImGuiCol_Text, to_imcolor(tooltip_palette->fg));
                    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColor{20, 20, 20, 200});
                    auto pos = window_origin + glm::vec2{10.0f};
                    ImGui::SetNextWindowPos(to_imgui(pos));
                    ImGui::BeginTooltip();
                    ImGui::Text("%s", tooltip);
                    ImGui::EndTooltip();
                    ImGui::PopStyleColor(2);
                }

                ImGui::EndChild();

                if (ImGui::IsMouseDragging(2)) {
                    auto delta = to_glm(io.MouseDelta);
                    origin += delta;
                }

                return changed;
            }
        }

        editor::editor()
            : tmp{std::make_unique<temporary_state>()}
        {}

        editor::~editor() = default;

        void editor::draw()
        {
            ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor{40, 40, 40});
            ImGui::PushStyleColor(ImGuiCol_PopupBg, ImColor{50, 50, 50});
            ImGui::PushStyleColor(ImGuiCol_Border, ImColor{30, 30, 30});
            ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImColor{0, 0, 0, 0});

            auto changed = draw_editor(g, previewing_node, scaling_level, grid_size, origin, default_node_width, *tmp);
            g.collect_garbage();
            engine::sanity_check(g);
            if (previewing_node != 0 && changed) {
                engine::execute(g, g.find_node(previewing_node));
            }

            ImGui::PopStyleColor(4);
        }
    }
}

