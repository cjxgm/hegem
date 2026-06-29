#include "../image/colorspace.hxx"
#include "rasterizer.hxx"
#include "state.hxx"
#include <set>
#include <unordered_set>
#include <array>

namespace hegem::swrast
{
    inline namespace
    {
        template <int N>
        auto generate_color_palette() -> std::array<image_type::color_type, N>
        {
            std::array<image_type::color_type, N> palette;
            for (int i=0; i<N; i++) {
                auto h = float(i) / float(N);
                palette[i] = image::colorspace::lch_to_rgb({30.0f, 50.0f, h}) * 10.0f;
            }
            return palette;
        }

        struct active_edge_order
        {
            auto operator () (edge_descriptor const& a, edge_descriptor const& b) const -> bool
            {
                if (a.x < b.x) return true;
                if (b.x < a.x) return false;
                return (a.dx < b.dx);
            }
        };

        using active_edge_list_type = std::multiset<edge_descriptor, active_edge_order>;

        auto rasterize(state s) -> image_type
        {
            if (s.edges.size() < 2)
                throw std::logic_error{"Invalid rasterization state."};

            image_type image{s.size};
            auto colors = generate_color_palette<32>();

            auto fill_scanline = [&] (polygon_id_type id, int y, int x0, int x1) {
                if (y < 0 || x1 < 0 || x0 >= s.size.x) return;
                if (x0 < 0) x0 = 0;
                if (x1 > s.size.x) x1 = s.size.x;
                if (x0 == x1) return;

                for (auto x=x0; x<x1; x++) {
                    auto color = (
                        id == 0
                        ? image_type::color_type{0.0f}
                        : colors[id % colors.size()]
                    );
                    image.put_clamp({x, y}, color);
                }
            };

            active_edge_list_type active_edges;

            auto y_front = begin(s.edges)->y_front; // left marker edge's y_front
            for (int y=y_front; y<s.size.y; y++) {
                for (auto first=begin(active_edges), last=end(active_edges); first != last;) {
                    auto it = first++;
                    if (y > it->y_back)
                        active_edges.erase(it);
                }

                for (auto first=begin(s.edges), last=end(s.edges); first != last;) {
                    auto it = first++;
                    if (it->y_front > y) break;
                    active_edges.emplace(std::move(*it));
                    s.edges.erase(it);
                }

                if (active_edges.size() < 2)
                    throw std::logic_error{"The 2 marker edges must be in active edges"};

                std::unordered_set<polygon_id_type> active_ids;
                for (auto first=begin(active_edges), last=--end(active_edges); first != last;) {
                    auto it = first++;

                    {   // toggle polygon (triangle) activeness
                        auto id = it->id;
                        auto id_it = active_ids.find(id);
                        if (id_it == end(active_ids)) {
                            active_ids.emplace(id);
                        } else {
                            active_ids.erase(id_it);
                        }

                        if (active_ids.empty())
                            throw std::logic_error{"The background should always be active"};
                    }

                    auto x0 = int(it->x);
                    auto x1 = int(first->x);
                    auto depth_sample = point_type{
                        (float(x0) + float(x1)) * 0.5f,
                        float(y),
                        0.0f,
                    };

                    auto id = *std::min_element(
                        begin(active_ids),
                        end(active_ids),
                        [&] (auto a, auto b) {
                            auto za = s.polys[a].z_at(depth_sample);
                            auto zb = s.polys[b].z_at(depth_sample);
                            return (za < zb);
                        });

                    fill_scanline(id, y, x0, x1);
                }

                auto history_edges = std::move(active_edges);
                for (auto e: history_edges) {
                    e.x += e.dx;
                    active_edges.emplace(std::move(e));
                }
            }

            return image;
        }
    }

    auto rasterize(scene::scene_type const& s, scene::view_type view) -> image_type
    {
        return rasterize(state{s, std::move(view)});
    }
}

