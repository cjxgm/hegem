#pragma once
#include "../lib/gl/gl.hh"
#include "../lib/glm/vec3.hh"
#include "../glu/resource.hh"

namespace rt::gui
{
    struct hdr_texture
    {
        int w;
        int h;
        glu::texture tex{};
        glm::vec3 blackpoint{0};
        glm::vec3 whitepoint{10};

        hdr_texture(int w, int h)
            :w{w}, h{h}
        {
            gl::uint_type id;
            gl::create_textures(gl::texture_2d, 1, &id);
            tex.reset(id);

            gl::texture_storage2d(id, 1, gl::rgba32f, w, h);

            float black[4]{0.0f, 0.0f, 0.0f, 1.0f};
            gl::clear_tex_image(id, 0, gl::rgba, gl::float_, black);
        }
    };

    void imgui_hdr_texture(hdr_texture* hdr);
}

