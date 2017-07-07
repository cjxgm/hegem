#include "cache.hh"
#include "scene.hh"
#include "node.hh"
#include <deque>

namespace rt::scene::cache_details
{
    cached_object::cached_object(material_id_type material_id, shape_type shape, glm::mat4 const& model_to_world)
        : material_id{material_id}
        , shape{std::move(shape)}
        , model_to_world{model_to_world}
        , world_to_model{glm::inverse(model_to_world)}
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
                    sc.objects.emplace_back(node.material_id, node.shape, xform);
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

