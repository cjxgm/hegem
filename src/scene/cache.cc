#include "cache.hh"
#include "scene.hh"
#include "node.hh"
#include <deque>
#include <cmath>

namespace rt::scene::cache_details
{
    namespace
    {
        shapes::mesh build_test_mesh(int subdivision, float thickness)
        {
            auto half_thickness = thickness * 0.5f;
            auto center_pos_index = subdivision * 4;
            auto center_neg_index = center_pos_index + 1;
            shapes::mesh m;

            shapes::direction_type normal_pos = glm::vec3{0.0f, 0.0f, 1.0f};
            shapes::direction_type normal_neg = glm::vec3{0.0f, 0.0f,-1.0f};
            for (int i=0; i<subdivision; i++) {
                auto this_pos_index = i * 4;
                auto next_pos_index = ((i + 1) % subdivision) * 4;
                auto this_pos_rim_index = this_pos_index + 1;
                auto next_pos_rim_index = next_pos_index + 1;
                auto this_neg_rim_index = this_pos_index + 2;
                auto next_neg_rim_index = next_pos_index + 2;
                auto this_neg_index = this_pos_index + 3;
                auto next_neg_index = next_pos_index + 3;

                auto a = static_cast<float>(M_PI * 2 / subdivision * i);
                auto p = glm::vec2{ std::cos(a), std::sin(a) };
                auto p_pos = glm::vec3{ p, half_thickness };
                auto p_neg = glm::vec3{ p,-half_thickness };
                shapes::direction_type normal_rim = glm::vec3{p, 0.0f};

                m.verts.push_back({ p_pos, normal_pos });
                m.verts.push_back({ p_pos, normal_rim });
                m.verts.push_back({ p_neg, normal_rim });
                m.verts.push_back({ p_neg, normal_neg });

                m.faces.emplace_back(center_pos_index, this_pos_index, next_pos_index);
                m.faces.emplace_back(center_neg_index, next_neg_index, this_neg_index);
                m.faces.emplace_back(this_pos_rim_index, this_neg_rim_index, next_neg_rim_index);
                m.faces.emplace_back(next_neg_rim_index, next_pos_rim_index, this_pos_rim_index);
            }
            auto p_pos = glm::vec3{ 0.0f, 0.0f, half_thickness };
            auto p_neg = glm::vec3{ 0.0f, 0.0f,-half_thickness };
            m.verts.push_back({ p_pos, normal_pos });
            m.verts.push_back({ p_neg, normal_neg });

            return m;
        }
    }

    cached_object::cached_object(material_id_type material_id, shape_type shape, glm::mat4 const& model_to_world)
        : material_id{material_id}
        , shape{std::move(shape)}
        , model_to_world{model_to_world}
        , world_to_model{glm::inverse(model_to_world)}
    {}

    cached_object::cached_object(material_id_type material_id, shape_type shape, glm::mat4 const& model_to_world, mesh_bvh_type bvh)
        : material_id{material_id}
        , shape{std::move(shape)}
        , model_to_world{model_to_world}
        , world_to_model{glm::inverse(model_to_world)}
        , opt_bvh{std::move(bvh)}
    {}

    scene_cache build_scene_cache(scene_type const& scene)
    {
        scene_cache sc;

        std::deque<node_type const*> pending_nodes{&scene.root};
        std::deque<glm::mat4> pending_xforms{glm::mat4{1.0f}};
        while (!pending_nodes.empty()) {
            auto& node = pending_nodes.front();
            auto& xform = pending_xforms.front();

            node->match(
                [&] (nodes::object const& node) {
                    node.shape.match(
                        [&] (shapes::mesh& m) {
                            mesh_bvh_type::face_soup_type face_ids;
                            auto face_count = static_cast<int>(m.faces.size());
                            face_ids.reserve(face_count);
                            for (int i=0; i<face_count; i++)
                                face_ids.emplace_back(i);

                            mesh_bvh_type bvh{mesh_face_trait{m}, std::move(face_ids)};
                            sc.objects.emplace_back(node.material_id, std::move(m), xform, std::move(bvh));
                        },
                        [&] (auto& shape) {
                            sc.objects.emplace_back(node.material_id, std::move(shape), xform);
                        });
                },
                [&] (nodes::xform const& node) {
                    pending_nodes.emplace_back(&node.node);
                    pending_xforms.emplace_back(xform * node.model);
                },
                [&] (nodes::group const& node) {
                    for (auto& node: node.nodes) {
                        pending_nodes.emplace_back(&node);
                        pending_xforms.emplace_back(xform);
                    }
                });

            pending_nodes.pop_front();
            pending_xforms.pop_front();
        }

        // inject test mesh
        {
            auto test_mesh = build_test_mesh(37, 0.05f);
            glm::mat4 test_xform{
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 3.0f, 1.0f,
            };
            mesh_bvh_type::face_soup_type face_ids;
            auto face_count = static_cast<int>(test_mesh.faces.size());
            face_ids.reserve(face_count);
            for (int i=0; i<face_count; i++)
                face_ids.emplace_back(i);

            mesh_bvh_type bvh{mesh_face_trait{test_mesh}, std::move(face_ids)};
            sc.objects.emplace_back(0, std::move(test_mesh), std::move(test_xform), std::move(bvh));
        }

        return sc;
    }
}

