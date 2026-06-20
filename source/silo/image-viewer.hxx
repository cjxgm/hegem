#pragma once
#include "../tool/constraints.hxx"
#include "../image/color.hxx"
#include "../image/image.hxx"
#include "../glu/resource.hxx"
#include <memory>

namespace hegem::silo
{
    struct image_viewer final: tool::constraints::only_movable
    {
        struct temporary_state;

        image_viewer();
        ~image_viewer(); // = default in implementation

        // If an image is not drawable, all operations to it becomes no-op.
        // No error will occur.
        auto drawable() const -> bool;

        auto texture() const -> glu::shared_texture2d;
        auto width() const -> int;
        auto height() const -> int;

        auto draw(float scale=1.0f) -> void;
        auto clear(image::color::linear_rgb color) -> void;
        auto resize(int w, int h) -> void;

        auto operator () (float scale=1.0f) { return draw(scale); }

    private:
        std::unique_ptr<temporary_state> tmp;
    };
}

