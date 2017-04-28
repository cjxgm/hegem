#pragma once
#include "../lib/gl/gl.hh"
#include "../lib/glm/vec3.hh"
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

        hdr_texture(int w, int h);

        void mark(util::tile tile);
        void unmark(util::tile tile);

    private:
        glu::shared_framebuffer fbo;
    };

    void imgui_hdr_texture(hdr_texture* hdr);
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

