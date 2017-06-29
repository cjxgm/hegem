#pragma once
#include "../lib/gl/gl.hh"
#include "../lib/glm/vec3.hh"
#include "../lib/glm/vec2.hh"
#include "../glu/resource.hh"
#include "../util/tile.hh"
#include <string>

namespace rt::app
{
    struct hdr_texture
    {
        int w;
        int h;
        glu::shared_texture2d tex;
        glu::shared_texture2d markers;
        glm::vec3 blackpoint{0};
        glm::vec3 whitepoint{10};
        float dither_amount{1};
        std::string name{"<unnamed>"};

        bool dragging{false};
        glm::vec2 drag_offset{};

        bool double_clicked{false};
        glm::vec2 image_local_clicked_pos{};

        hdr_texture(int w, int h);

        void mark(util::tile tile);
        void unmark(util::tile tile);

    private:
        glu::shared_framebuffer fbo;
    };

    void imgui_hdr_texture(hdr_texture* hdr, char const* drag_receiver="drag receiver");
    void imgui_hdr_color(
            char const* color_label,
            char const* intensity_label,
            glm::vec3* color,
            float intensity_speed=1,
            float intensity_min=0,
            float intensity_max=1000,
            char const* intensity_format="%.3f",
            float intensity_power = 10);
}

