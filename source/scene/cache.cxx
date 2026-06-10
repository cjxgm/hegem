#include "../hegem/mesh.hxx"
#include "cache.hxx"
#include "scene.hxx"
#include "node.hxx"
#include <deque>

namespace rt::scene::cache_details
{
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
                    node.shape
                        .match(
                            [&] (shapes::hemesh const& hm) -> shape_type {
                                return build_mesh(hm, 1e-5f);
                            },
                            [&] (auto shape) -> shape_type { return std::move(shape); })
                        .match(
                            [&] (shapes::mesh m) {
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

        return sc;
    }
}

