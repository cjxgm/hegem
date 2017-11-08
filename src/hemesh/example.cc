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
    }
}

