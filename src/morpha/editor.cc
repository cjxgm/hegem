#include "../lib/imgui.hh"
#include "../lib/glm/vec2.hh"
#include "../lib/glm/op/common.hh"
#include "../lib/gl/gl.hh"
#include "../image/image.hh"
#include "../app/app.hh"
#include "editor.hh"
#include "progress-chooser.hh"
#include "polar-path.hh"
#include "image-viewer.hh"
#include "file-slot.hh"
#include "morph.hh"
#include <vector>
#include <memory>
#include <stdexcept>

namespace rt::morpha
{
    inline namespace editor_details
    {
        struct feature
        {
            polar_path path;
            polar_path_cache cache;

            void update_cache() { update_polar_path_cache(path, cache); }
        };

        auto scaling_factor(float& scaling_level) -> float
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

            scaling_level = glm::clamp(scaling_level, -scaler_upperbound, scaler_upperbound);
            auto selector = int(glm::abs(scaling_level));
            auto scaler = scalers[selector];
            if (scaling_level < 0.0f) scaler = 1.0f / scaler;
            return scaler;
        }
    }

    struct editor::temporary_state
    {
        int morphing_progress{};
        bool interpolated_features_needs_update{};
        bool morphing_needs_update{};
        std::vector<feature> features0;
        std::vector<feature> features1;
        std::vector<feature> features_tmp;

        float scaling_level{};
        float scaling{1.0f};
        glm::vec2 origin{0.0f, 0.0f};

        image_viewer preview;
        file_slot file0;
        file_slot file1;
        std::shared_ptr<image::image_rgb> image0;
        std::shared_ptr<image::image_rgb> image1;

        int (&tile_size)[2];

        temporary_state(int (&tile_size)[2])
            : tile_size{tile_size}
        {
            features0.emplace_back();
            features1.emplace_back();
        }

        void update_interpolated_features()
        {
            features_tmp.resize(features0.size());
            auto it = features_tmp.begin();
            auto it0 = features0.begin();
            auto it1 = features1.begin();
            auto last0 = features0.end();
            auto last1 = features1.end();
            auto amount = float(morphing_progress) / 100.0f;

            for (; it0 != last0 && it1 != last1; ++it0, ++it1, ++it) {
                update_polar_path_interpolation(it0->path, it1->path, amount, it->path);
                it->update_cache();
            }
        }

        void add_feature_point(glm::vec2 pos)
        {
            auto&  active = (morphing_progress == 0 ? features0 : features1).back();
            auto& passive = (morphing_progress == 0 ? features1 : features0).back();

            auto base_pos = (
                active.cache.empty()
                ? glm::vec2{0.0f, 0.0f}
                : active.cache.back().pos
            );
            auto base_angle = (
                active.cache.empty()
                ? 0.0f
                : active.cache.back().angle_sum_so_far
            );
            auto& vert = active.path.emplace_back(pos - base_pos, -base_angle);
            passive.path.emplace_back(vert);

            active.update_cache();
            passive.update_cache();
        }

        void move_last_polar_vertex_to(glm::vec2 pos)
        {
            auto&  active = (morphing_progress == 0 ? features0 : features1).back();
            auto& passive = (morphing_progress == 0 ? features1 : features0).back();

            if (active.path.empty()) return;

            move_polar_vertex_to(active.path, active.cache, active.path.size() - 1, pos);
            passive.path.back() = active.path.back();

            active.update_cache();
            passive.update_cache();
        }

        void commit_feature_path()
        {
            if (features0.back().path.size() > 1) {
                features0.emplace_back();
                features1.emplace_back();
            } else {
                features0.back().path.clear();
                features1.back().path.clear();
                features0.back().cache.clear();
                features1.back().cache.clear();
            }
        }

        void re_edit_polar_path_after_vertex(int polar_path_index, int vertex_index)
        {
            commit_feature_path();
            if (polar_path_index == int(features0.size())) return;
            if (features0.size() == 1) return;

            features0.pop_back();
            features1.pop_back();
            std::swap(features0[polar_path_index], features0.back());
            std::swap(features1[polar_path_index], features1.back());

            auto& feature0 = features0.back();
            auto& feature1 = features1.back();
            feature0.path.resize(vertex_index);
            feature1.path.resize(vertex_index);
            feature0.cache.resize(vertex_index);
            feature1.cache.resize(vertex_index);
        }

        auto build_morphing_cache() -> morphing_cache
        {
            update_interpolated_features();
            if (features0.size() != features1.size() || features0.size() != features_tmp.size())
                throw std::logic_error{"features sizes differ"};

            morphing_cache cache;

            auto it0 = features0.begin();
            auto it1 = features1.begin();
            auto it_tmp = features_tmp.begin();
            auto last0 = features0.end();
            for (; it0 != last0; ++it0, ++it1, ++it_tmp)
                extend_morphing_cache(it0->cache, it_tmp->cache, it1->cache, cache);

            return cache;
        }
    };

    namespace
    {
        auto edit_feature_paths(
            editor::temporary_state& tmp,
            glm::vec2 window_origin,
            int* hovered_path_index=nullptr,
            int* hovered_vertex_index=nullptr
        ) -> bool
        {
            if (tmp.morphing_progress != 0 && tmp.morphing_progress != 100) {
                if (tmp.interpolated_features_needs_update) {
                    tmp.interpolated_features_needs_update = false;
                    tmp.update_interpolated_features();
                }

                auto& features = tmp.features_tmp;
                for (auto& feature: features) {
                    auto active = (&feature == &features.back());
                    edit_polar_path(feature.path, feature.cache, active, true, tmp.origin + window_origin, tmp.scaling);
                }

                return false;
            }

            auto& features = (tmp.morphing_progress == 0 ? tmp.features0 : tmp.features1);
            auto changed = false;
            auto path_index = 0;
            for (auto& feature: features) {
                auto active = (&feature == &features.back());
                int hovered_index = -1;

                if (edit_polar_path(feature.path, feature.cache, active, false, tmp.origin + window_origin, tmp.scaling, &hovered_index)) {
                    changed = true;
                    feature.update_cache();
                }

                if (hovered_index != -1) {
                    if (hovered_vertex_index) *hovered_vertex_index = hovered_index;
                    if (hovered_path_index) *hovered_path_index = path_index;
                }

                path_index++;
            }
            return changed;
        };

        auto canvas(editor::temporary_state& tmp) -> bool
        {
            auto changed = false;
            auto& io = ImGui::GetIO();

            // color palette
            const auto origin_color = glm::vec3{1.0f, 0.5f, 0.5f};

            ImGui::BeginChild("morpha canvas", {}, {}, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            {
                auto window_origin = glm::vec2{ImGui::GetCursorScreenPos()};
                //auto origin_offset = glm::vec2{ImGui::GetCursorPos()};
                auto mouse_screen_pos = glm::vec2{ImGui::GetMousePos()};
                auto mouse_local_pos = mouse_screen_pos - window_origin;
                auto& draw_list = *ImGui::GetWindowDrawList();

                auto local_to_screen = [&] (glm::vec2 local) {
                    return local + window_origin;
                };

                {   // zooming and panning control
                    if (ImGui::IsWindowHovered() && io.MouseWheel != 0.0f) {
                        auto new_scaling = scaling_factor(tmp.scaling_level += io.MouseWheel);
                        if (tmp.scaling != new_scaling) {
                            tmp.origin = (tmp.origin - mouse_local_pos) * (new_scaling / tmp.scaling) + mouse_local_pos;
                            tmp.scaling = new_scaling;
                        }
                    }

                    static auto panning = false;
                    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && ImGui::IsMouseClicked(2)) panning = true;
                    if (!ImGui::IsMouseDown(2)) panning = false;
                    if (panning && ImGui::IsMouseDragging(2)) {
                        tmp.origin += glm::vec2{io.MouseDelta};
                    }

                    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && ImGui::IsMouseDoubleClicked(2)) {
                        tmp.origin = {};
                        tmp.scaling = 1.0f;
                        tmp.scaling_level = 0.0f;
                    }
                }

                {   // draw origin
                    auto radius = tmp.scaling * 10;
                    auto width = glm::min(radius * 0.3f, 3.0f);
                    auto origin = local_to_screen(tmp.origin);
                    auto a0 = origin + glm::vec2{-radius, 0.0f};
                    auto a1 = origin + glm::vec2{+radius, 0.0f};
                    auto b0 = origin + glm::vec2{0.0f, -radius};
                    auto b1 = origin + glm::vec2{0.0f, +radius};
                    auto color = ImGui::ColorConvertFloat4ToU32(origin_color);
                    draw_list.AddLine(a0, a1, color, width);
                    draw_list.AddLine(b0, b1, color, width);
                }

                {   // draw image preview
                    ImGui::SetCursorPos(tmp.origin);
                    tmp.preview(tmp.scaling);
                }

                int hovered_path_index = -1;
                int hovered_vertex_index = -1;
                changed |= edit_feature_paths(tmp, window_origin, &hovered_path_index, &hovered_vertex_index);

                {
                    ImGui::SetCursorPos({0, 0});
                    ImGui::InvisibleButton("add vertex", ImGui::GetWindowSize());
                    if (tmp.morphing_progress == 0 || tmp.morphing_progress == 100) {
                        if (ImGui::IsItemClicked(0)) {
                            auto pos = (mouse_local_pos - tmp.origin) / tmp.scaling;
                            tmp.add_feature_point(pos);
                            changed = true;
                        }
                        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
                            auto pos = (mouse_local_pos - tmp.origin) / tmp.scaling;
                            tmp.move_last_polar_vertex_to(pos);
                            if (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f)
                                changed = true;
                        }
                        if (ImGui::IsItemClicked(1)) {
                            if (hovered_path_index == -1) {
                                tmp.commit_feature_path();
                            } else {
                                tmp.re_edit_polar_path_after_vertex(hovered_path_index, hovered_vertex_index);
                            }
                            changed = true;
                        }
                    }
                }
            }
            ImGui::EndChild();

            return changed;
        }

        void update_morphing(editor::temporary_state& tmp)
        {
            if (!tmp.image0 && !tmp.image1) return;
            if (!tmp.morphing_needs_update) return;
            tmp.morphing_needs_update = false;

            auto& image0 = (tmp.image0 ? tmp.image0 : tmp.image1);
            auto& image1 = (tmp.image1 ? tmp.image1 : tmp.image0);
            auto amount = float(tmp.morphing_progress) / 100.0f;
            if (&image0 == &image1) amount = 0.0f;

            auto cache = std::make_shared<morphing_cache>(tmp.build_morphing_cache());

            auto make_task = [&] (auto tile) -> app::task_type {
                return [
                    preview_tex=tmp.preview.texture(),
                    cache,
                    image0,
                    image1,
                    tile,
                    amount
                ] (auto tx, auto shared_canceled) {
                    auto result = morph(*image0, *image1, *cache, amount, tile);
                    tx.send(util::possibly_canceled_job{
                        shared_canceled,
                        [image=std::move(result), tex=std::move(preview_tex), tile] () {
                            gl::texture_sub_image2d(
                                tex,
                                0,
                                tile.x, tile.y, tile.w, tile.h,
                                gl::rgb, gl::float_,
                                image.data()
                            );
                        },
                    });
                };
            };

            auto preview_size = glm::max(image0->size(), image1->size());
            if (tmp.preview.width() != preview_size.x || tmp.preview.height() != preview_size.y) {
                tmp.preview.resize(preview_size.x, preview_size.y);
                tmp.preview.clear({});
            }

            auto [tile_w, tile_h] = tmp.tile_size;
            app::task_group_type tasks;
            for (auto& tile: util::tile_iterator{tile_w, tile_h, preview_size.x, preview_size.y}) {
                tasks.emplace_back(make_task(tile));
            }

            app::schedule_tasks(std::move(tasks));
        }
    }

    editor::editor(int (&tile_size)[2]): tmp{std::make_unique<temporary_state>(tile_size)} {}
    editor::~editor() = default;

    void editor::draw()
    {
        if (auto& filename = tmp->file0("morphing source", "/usr/share/hegem/support/morphing"); !filename.empty()) {
            tmp->image0 = std::make_shared<image::image_rgb>(image::load(filename));
            tmp->morphing_progress = 0;
            tmp->interpolated_features_needs_update = true;
            tmp->morphing_needs_update = true;
        }
        if (auto& filename = tmp->file1("morphing target", "/usr/share/hegem/support/morphing"); !filename.empty()) {
            tmp->image1 = std::make_shared<image::image_rgb>(image::load(filename));
            tmp->morphing_progress = 100;
            tmp->interpolated_features_needs_update = true;
            tmp->morphing_needs_update = true;
        }
        ImGui::Spacing();

        if (progress_chooser(&tmp->morphing_progress)) {
            tmp->interpolated_features_needs_update = true;
            tmp->morphing_needs_update = true;
        }

        if (canvas(*tmp)) {
            if (bool(tmp->image0) ^ bool(tmp->image1))
                tmp->morphing_needs_update = true;
        }

        update_morphing(*tmp);
    }
}

