#include "../lib/glm/op/common.hxx"
#include "../lib/imgui.hxx"
#include "polar-path.hxx"

namespace hegem::silo
{
    void update_polar_path_cache(polar_path const& path, polar_path_cache& cache)
    {
        cache.clear();
        cache.reserve(path.size());

        if (path.empty()) return;

        auto it = path.begin();
        auto last = path.end();

        auto cartesian_base = cache.emplace_back(*it, 0.0f).pos;
        auto angle_base = 0.0f;

        while (++it != last) {
            auto vert = it->rotate(angle_base);
            angle_base += it->angle;

            auto& entry = cache.emplace_back(vert, angle_base);
            cartesian_base = (entry.pos += cartesian_base);
        }
    }

    void update_polar_path_interpolation(
        polar_path const& src0,
        polar_path const& src1,
        float x,
        polar_path& dst)
    {
        dst.clear();
        if (src0.empty()) return;

        auto it0 = src0.begin();
        auto it1 = src1.begin();
        auto last0 = src0.end();
        auto last1 = src1.end();

        {   // interpolate the first point in cartesian coordinates.
            auto p0 = glm::vec2{*it0++};
            auto p1 = glm::vec2{*it1++};
            auto p = glm::mix(p0, p1, x);
            dst.emplace_back(p);
        }

        for (; it0 != last0 && it1 != last1; ++it0, ++it1) {
            auto angle  = glm::mix(it0->angle,  it1->angle,  x);
            auto length = glm::mix(it0->length, it1->length, x);
            dst.emplace_back(angle, length);
        }
    }

    void move_polar_vertex_to(polar_path& path, polar_path_cache const& cache, int vertex_idx, polar_vertex::cartesian_type pos)
    {
        auto base_pos = (vertex_idx == 0 ? polar_vertex::cartesian_type{0.0f, 0.0f} : cache[vertex_idx-1].pos);
        auto base_angle = (vertex_idx == 0 ? 0.0f : cache[vertex_idx-1].angle_sum_so_far);
        auto offset = pos - base_pos;
        path[vertex_idx] = polar_vertex{offset, -base_angle};
    }

    void move_polar_vertex_locally_to(polar_path& path, polar_path_cache const& cache, int vertex_idx, polar_vertex::cartesian_type pos)
    {
        move_polar_vertex_to(path, cache, vertex_idx, pos);
        if (vertex_idx+1 < int(path.size())) {
            auto next_pos = glm::vec2{cache[vertex_idx+1].pos};
            auto base_pos = pos;
            auto base_angle = (vertex_idx == 0 ? 0.0f : cache[vertex_idx-1].angle_sum_so_far + path[vertex_idx].angle);
            auto offset = next_pos - base_pos;
            path[vertex_idx+1] = polar_vertex{offset, -base_angle};

            if (vertex_idx+2 < int(path.size())) {
                auto next_pos2 = glm::vec2{cache[vertex_idx+2].pos};
                auto base_pos2 = next_pos;
                auto base_angle2 = base_angle + path[vertex_idx+1].angle;
                auto offset2 = next_pos2 - base_pos2;
                path[vertex_idx+2] = polar_vertex{offset2, -base_angle2};
            }
        }
    }

    auto edit_polar_path(
        polar_path& path,
        polar_path_cache const& cache,
        bool active,
        bool readonly,
        glm::vec2 origin,
        float scaling,
        int* hovered_vertex_index
    ) -> bool
    {
        if (path.empty()) return false;

        const auto alpha = 0.9f * (readonly ? 0.5f : 1.0f);
        const auto base_color = (active ? glm::vec3{0.5f, 1.0f, 0.5f} : glm::vec3{0.5f, 0.5f, 1.0f});
        const auto base_accent_color = (active ? glm::vec3{0.7f, 1.0f, 0.9f} : glm::vec3{1.0f, 0.8f, 0.5f});
        const auto base_size = (readonly ? 3.5f : 5.0f);

        const auto palette_edge_color = glm::vec4{base_color * 0.8f, alpha};
        const auto palette_nudge_color = glm::vec4{base_color, alpha};
        const auto palette_front_nudge_color = glm::vec4{base_accent_color, alpha};
        const auto palette_line_width = base_size * 0.6f;
        const auto palette_nudge_radius = base_size;

        auto& io = ImGui::GetIO();
        auto& draw_list = *ImGui::GetWindowDrawList();
        auto changed = false;

        if (cache.size() > 1) {
            auto it = cache.begin();
            auto last = cache.end();
            auto old = it++;
            for (; it != last; ++it, ++old) {
                auto p0 = old->pos * scaling + origin;
                auto p1 =  it->pos * scaling + origin;
                auto width = glm::min(scaling * palette_line_width, palette_line_width);
                auto edge_color = ImGui::ColorConvertFloat4ToU32(palette_edge_color);
                draw_list.AddLine(p0, p1, edge_color, width);
            }
        }

        auto vertex_index = 0;
        for (auto& entry: cache) {
            auto center = entry.pos * scaling + origin;
            auto radius = glm::min(scaling * palette_nudge_radius, palette_nudge_radius);
            auto nudge_color_linear = (vertex_index == 0 ? palette_front_nudge_color : palette_nudge_color);

            if (!readonly) {
                auto btn_pos = center - glm::vec2{radius, radius}*2.0f;
                auto btn_size = glm::vec2{radius, radius} * 4.0f;

                ImGui::SetCursorScreenPos(btn_pos);
                ImGui::PushID(&entry);
                ImGui::InvisibleButton("move", btn_size);
                ImGui::PopID();

                if (ImGui::IsItemHovered() || ImGui::IsItemActive()) radius *= 2.0f;
                if (ImGui::IsItemActive()) nudge_color_linear.a *= 0.2f;
                if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
                    auto mouse_screen_pos = glm::vec2{ImGui::GetMousePos()};
                    auto pos = (mouse_screen_pos - origin) / scaling;
                    if (io.KeyShift) {
                        move_polar_vertex_locally_to(path, cache, vertex_index, pos);
                    } else {
                        move_polar_vertex_to(path, cache, vertex_index, pos);
                    }
                    if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f)
                        changed = true;
                }
                if (hovered_vertex_index && ImGui::IsItemHovered())
                    *hovered_vertex_index = vertex_index;
            }

            auto nudge_color = ImGui::ColorConvertFloat4ToU32(nudge_color_linear);
            draw_list.AddCircleFilled(center, radius, nudge_color);

            vertex_index++;
        }

        return changed;
    }
}

