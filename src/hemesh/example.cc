#include "../lib/glm/vec3.hh"
#include "../lib/glm/op/transform.hh"
#include "example.hh"
#include "hemesh.hh"
#include "mesh.hh"
#include "dump.hh"
#include "op.hh"
#include <iostream>

namespace rt::hemesh
{
    inline namespace example
    {
        hemesh make_example()
        {
            rt::hemesh::hemesh m;

            auto cube = make_cube(m)->ring->face->body;
            auto cylinder = make_polygon_cylinder(m, 12, 1.0f, 4, 2.0f)->ring->face->body;

            // ">>" shape (it's actually "<<" shape)
            //  _______
            //   \     \
            //    \     \
            //    /     /
            //   /_____/
            auto hdisk = make_polygon_disk(m, 6, 1.0f);
            auto arrow = hdisk->ring->face->body;
            hdisk->start->pos = {};
            extrude(m, hdisk->ring, {0, 2, 0});

            dump_pretty(m);

            affine_transform(cube, glm::translate(glm::vec3{ 0.0f, 0.0f, -2.0f }));
            affine_transform(arrow, glm::translate(glm::vec3{ -2.0f, 0.0f, 0.0f }));
            affine_transform(cylinder, glm::translate(glm::vec3{ 0.0f, 0.5f, 0.0f }));
            affine_transform_all(cube, glm::translate(glm::vec3{ 1.0f, -1.0f, 1.0f }));

            auto tm = build_mesh(m);
            write_obj(tm, std::cerr);
            write_obj(tm, "/tmp/test.obj");

            return m;
        }

        scene::scene_type make_example_scene()
        {
            auto view = scene::view_type {
                "Example",
                { 800, 450 },
                8,
                4,
                scene::cameras::pin_hole {
                    glm::vec3{ 0.0f, 0.0f, 5.0f },
                    glm::vec3{ 0.0f, 0.0f, -1.0f },
                    glm::vec3{ 0.0f, 1.0f, 0.0f },
                    glm::radians(30.0f),
                },
            };

            auto lamp_main = scene::lamps::sun {
                glm::vec3{ 2.0f, -2.0f, -1.0f },
                glm::vec3{ 1.0f, 0.9f, 0.8f } * 20.0f,
            };
            auto lamp_rim = scene::lamps::sun {
                glm::vec3{ -1.0f, 1.0f, -1.0f },
                glm::vec3{ 0.2f, 0.4f, 1.0f } * 16.0f,
            };
            auto lamp_back = scene::lamps::sun {
                glm::vec3{ -0.2f, 0.2f, 0.8f },
                glm::vec3{ 0.6f, 0.6f, 0.6f } * 10.0f,
            };

            auto mat_outline = scene::materials::physically_based {
                scene::texture_packs::pure{},
                glm::vec3{0.0f, 0.0f, 0.0f},
                glm::vec3{1.0f, 1.0f, 1.0f} * 0.5f,
                0.1f,
                1.5f,
            };
            auto mat_object = scene::materials::physically_based {
                scene::texture_packs::pure{},
                glm::vec3{1.0f, 0.4f, 0.1f},
                glm::vec3{1.0f, 1.0f, 1.0f} * 0.5f,
                0.01f,
                1.5f,
            };
            auto mat_sky = scene::materials::solid_color {
                glm::vec3{0.3f, 0.3f, 1.0f} * 10.0f,
            };

            scene::nodes::group node_root;
            auto example = make_example();
            node_root.nodes.emplace_back(
                scene::nodes::object {
                    2,
                    build_outline_mesh(example),
                });
            node_root.nodes.emplace_back(
                scene::nodes::object {
                    1,
                    std::move(example),
                });

            auto scene = scene::scene_type {
                "hemesh",
                { std::move(view) },
                { std::move(lamp_main), std::move(lamp_rim), std::move(lamp_back) },
                { std::move(mat_sky), std::move(mat_object), std::move(mat_outline) },
                std::move(node_root),
                0,
            };

            scene.rebuild_cache();

            return scene;
        }
    }
}

