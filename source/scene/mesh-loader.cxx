#include "../lib/tiny-obj-loader.inl"
#include "../lib/glm/vec3.hxx"
#include "../lib/glm/op/geom.hxx"
#include "mesh-loader.hxx"
#include <vector>
#include <stdexcept>
#include <iostream>     // TODO: use journal

namespace rt::scene::mesh_loader
{
    shapes::mesh from_path(util::as_czstring path)
    {
        shapes::mesh m;
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;

        {   // Load wavefront obj file, ignoring materials
            std::vector<tinyobj::material_t> materials;
            std::string err;
            bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.data());

            if (!err.empty()) std::cerr << "Error loading mesh " << path.data() << ": " << err << "\n";
            if (!ok) throw std::runtime_error{err};
        }

        if (shapes.size() > 1)
            std::cerr << "Only the first shape will be loaded from mesh " << path.data() << "\n";

        if (shapes.size() == 0)
            throw std::runtime_error{"Mesh must contain at least 1 shape"};

        // TODO optimization: don't duplicate every attribute
        auto& mshape = shapes[0].mesh;
        int face_count = mshape.num_face_vertices.size();
        for (int face=0; face < face_count; face++) {
            auto idx = &mshape.indices[3 * face];
            int face_vert_base = m.verts.size();
            for (int vert=0; vert < 3; vert++) {
                auto pos = &attrib.vertices[3 * idx[vert].vertex_index];
                auto normal = &attrib.normals[3 * idx[vert].normal_index];
                m.verts.push_back({
                    glm::vec3{pos[0], pos[1], pos[2]},
                    glm::vec3{normal[0], normal[1], normal[2]},
                });
            }
            m.faces.emplace_back(face_vert_base, face_vert_base+1, face_vert_base+2);
        }

        return m;
    }
}

