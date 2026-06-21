#include "../lib/glm/vec3.hxx"
#include "../lib/glm/op/transform.hxx"
#include "example.hxx"
#include "hemesh.hxx"
#include "mesh.hxx"
#include "dump.hxx"
#include "op.hxx"
#include <iostream>
#include <stdexcept>

namespace hegem::swing
{
    inline namespace example
    {
        hemesh make_example()
        {
            hemesh m;
            m.diagnose("just initialized");

            auto cube_hege = make_cube(m);
            auto cube = cube_hege->ring->face->body;
            auto cube_top = cube_hege->next->next->twin->ring->face;
            {
                auto hole = make_polygon_disk(m, 6, 0.2f, cube_top)->ring;
                affine_transform(hole, glm::translate(glm::vec3{ 0.0f, 1.0f, 0.0f }));
                extrude(m, hole->face, { 0.0f, 0.5f, 0.0f });
            }
            m.diagnose("after a cube's creation");

            auto cylinder_face = make_polygon_disk(m, 12, 1.0f)->ring->face;
            auto cylinder_counter_face = cylinder_face->boundary->any_hege->twin->ring->face;
            auto cylinder = cylinder_face->body;
            {
                auto hole0 = make_polygon_disk(m, 4, 0.2f, cylinder_face, cylinder_counter_face)->ring;
                auto hole1 = make_polygon_disk(m, 6, 0.5f, cylinder_face, cylinder_counter_face)->ring;
                hole1->any_hege->next->next->next->start->pos = {};
                affine_transform(hole0, glm::translate(glm::vec3{ -0.3f, 0.0f, 0.0f }));
                affine_transform(hole1, glm::translate(glm::vec3{ 0.3f, 0.0f, 0.0f }));
            }
            extrude(m, cylinder_face, {  0.15f, 0.5f, 0.0f });
            extrude(m, cylinder_face, {  0.05f, 0.5f, 0.0f });
            extrude(m, cylinder_face, { -0.05f, 0.5f, 0.0f });
            extrude(m, cylinder_face, { -0.15f, 0.5f, 0.0f });
            m.diagnose("after a cylinder's creation");

            auto disk_face = make_polygon_disk(m, 12, 1.0f)->ring->face;
            auto disk_counter_face = disk_face->boundary->any_hege->twin->ring->face;
            auto disk = disk_face->body;
            {
                auto hole0 = make_polygon_disk(m, 4, 0.2f, disk_face, disk_counter_face)->ring;
                auto hole1 = make_polygon_disk(m, 5, 0.3f, disk_face, disk_counter_face)->ring;
                affine_transform(hole0, glm::translate(glm::vec3{ -0.3f, 0.0f, 0.0f }));
                affine_transform(hole1, glm::translate(glm::vec3{ 0.3f, 0.0f, 0.0f }));
            }
            m.diagnose("after a disk's creation");

            // ">>" shape (it's actually "<<" shape)
            auto hdisk = make_polygon_disk(m, 6, 1.0f);
            auto arrow = hdisk->ring->face->body;
            hdisk->start->pos = {};
            extrude(m, hdisk->ring->face, {0, 2, 0});
            m.diagnose("after an arrow's creation");

            affine_transform(cube, glm::translate(glm::vec3{ 0.0f, 0.0f, -2.0f }));
            affine_transform(arrow, glm::translate(glm::vec3{ -2.0f, 0.0f, 0.0f }));
            affine_transform(cylinder, glm::translate(glm::vec3{ 0.0f, 0.5f, 0.0f }));
            affine_transform(disk, glm::translate(glm::vec3{ 2.5f, 1.0f, 0.0f }));
            affine_transform_all(cube, glm::translate(glm::vec3{ 0.0f, -1.0f, 0.0f }));

            return m;
        }

        scene::scene_type make_example_scene()
        {
            auto view = scene::view_type {
                "Example",
                { 800, 450 },
                2,
                16,
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
                glm::vec3{1.0f, 1.0f, 1.0f} * 0.8f,
                0.1f,
                1.5f,
            };
            auto mat_object = scene::materials::physically_based {
                scene::texture_packs::pure{},
                glm::vec3{1.0f, 0.4f, 0.1f},
                glm::vec3{1.0f, 1.0f, 1.0f} * 0.2f,
                0.05f,
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
                "swing",
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

