#include "cache.hh"
#include "scene.hh"
#include "node.hh"
#include <deque>
#include <cmath>

namespace rt::scene::cache_details
{
    namespace
    {
        shapes::mesh build_test_mesh(int subdivision)
        {
            shapes::mesh m;
            shapes::direction_type normal = glm::vec3{0.0f, 0.0f, 1.0f};
            for (int i=0; i<subdivision; i++) {
                float a = M_PI * 2 / subdivision * i;
                m.verts.push_back({ glm::vec3{std::cos(a), std::sin(a), 0.0f}, normal });
                m.faces.emplace_back(i, (i+1) % subdivision, subdivision);
            }
            m.verts.push_back({ glm::vec3{0.0f, 0.0f, 0.0f}, normal });
            return m;
        }
    }

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

        // inject test mesh
        auto test_mesh = build_test_mesh(37);
        glm::mat4 test_xform{
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 3.0f, 1.0f,
        };
        sc.objects.emplace_back(0, std::move(test_mesh), std::move(test_xform));

        return sc;
    }
}

