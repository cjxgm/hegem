#pragma once
#include "../lib/gl/gl.hh"
#include "../lib/glm/vec3.hh"
#include "../glu/resource.hh"
#include <string>

namespace rt::app
{
    struct hdr_texture
    {
        int w;
        int h;
        glu::texture tex{};
        glm::vec3 blackpoint{0};
        glm::vec3 whitepoint{10};
        float dither_amount{1};
        std::string name{"<unnamed>"};

        hdr_texture(int w, int h)
            :w{w}, h{h}
        {
            gl::uint_type id;
            gl::create_textures(gl::texture_2d, 1, &id);
            tex.reset(id);

            gl::texture_storage2d(id, 1, gl::rgba32f, w, h);

            float black[4]{0.0f, 0.0f, 0.0f, 1.0f};
            gl::clear_tex_image(id, 0, gl::rgba, gl::float_, black);

            gl::texture_parameteri(id, gl::texture_min_filter, gl::linear);
            gl::texture_parameteri(id, gl::texture_mag_filter, gl::nearest);
        }
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

