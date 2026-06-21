#pragma once

namespace hegem::tool
{
    namespace tile_details
    {
        struct tile
        {
            int x;
            int y;
            int w;
            int h;
        };

        // only works in range-for
        struct tile_iterator
        {
            tile_iterator(int tile_w, int tile_h, int canvas_w, int canvas_h);
            tile_iterator() : tile_w{-1} {}

            auto& begin() const { return *this; }
            auto end() { return tile_iterator{}; }
            auto& operator * () const { return state; }
            auto& operator++() { next(); return *this; }

            // Only if `a` and `b` both have reached end status were they equal.
            friend bool operator == (tile_iterator const& a, tile_iterator const& b);
            friend bool operator != (tile_iterator const& a, tile_iterator const& b) { return !(a == b); }

        private:
            tile state;
            int i;
            int tile_max_x;
            int tile_max_y;
            int tile_w;
            int tile_h;
            int canvas_w;
            int canvas_h;

            bool is_end_iterator() const { return tile_w < 0; }
            bool iteration_end() const;
            auto next() -> void;
        };
    }

    using tile_details::tile_iterator;
    using tile_details::tile;
}

