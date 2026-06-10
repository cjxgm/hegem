#include "tile.hxx"

namespace rt::util::tile_details
{
    tile_iterator::tile_iterator(int tile_w, int tile_h, int canvas_w, int canvas_h)
        : state{}
        , i{-1}
        , tile_w{tile_w}
        , tile_h{tile_h}
        , canvas_w{canvas_w}
        , canvas_h{canvas_h}
    {
        tile_max_x = canvas_w / tile_w + (canvas_w % tile_w != 0);
        tile_max_y = canvas_h / tile_h + (canvas_h % tile_h != 0);
        next();
    }

    void tile_iterator::next()
    {
        if (iteration_end()) return;
        i++;

        auto tile_x = i % tile_max_x;
        auto tile_y = i / tile_max_x;
        state.x = tile_x * tile_w;
        state.y = tile_y * tile_h;
        state.w = state.x + tile_w > canvas_w ? canvas_w - state.x : tile_w;
        state.h = state.y + tile_h > canvas_h ? canvas_h - state.y : tile_h;
    }

    bool tile_iterator::iteration_end() const
    {
        return i == tile_max_x * tile_max_y;
    }

    bool operator == (tile_iterator const& a, tile_iterator const& b)
    {
        auto enda = a.is_end_iterator() || a.iteration_end();
        auto endb = b.is_end_iterator() || b.iteration_end();
        return enda && endb;
    }
}

