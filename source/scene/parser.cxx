#include "../lib/glm/vec2.hxx"
#include "../lib/glm/vec3.hxx"
#include "../lib/glm/vec4.hxx"
#include "../lib/glm/mat4.hxx"
#include "../lib/glm/op/trig.hxx"
#include "../lib/glm/op/common.hxx"
#include "../lib/glm/op/geom.hxx"
#include "parser.hxx"
#include "camera.hxx"
#include "view.hxx"
#include "material.hxx"
#include "shape.hxx"
#include "node.hxx"
#include "lamp.hxx"
#include "mesh-loader.hxx"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace hegem::scene
{
    namespace
    {
        using material_container_type = scene_type::material_container_type;
        using lamp_container_type = scene_type::lamp_container_type;
        using view_container_type = scene_type::view_container_type;
        using group_node_container_type = nodes::group::node_container_type;

        inline namespace literal_to_string_details
        {
            template <class T, class = std::enable_if_t<std::is_arithmetic<T>::value>>
            auto literal_to_string(T&& x)
            {
                std::ostringstream oss;
                oss << x;
                return oss.str();
            }

            auto literal_to_string(char const* x)
            {
                std::ostringstream oss;
                oss << std::quoted(x);
                return oss.str();
            }

            auto literal_to_string(std::string const& x)
            {
                return literal_to_string(x.data());
            }
        }

        inline namespace expect_details
        {
            template <class T>
            void expect(std::istream & ist, T&& x, std::string const& name)
            {
                T v;
                if ((ist >> v) && v == x) return;
                throw std::runtime_error{"literal " + literal_to_string(x) + " expected for " + name};
            }

            void expect(std::istream & ist, char const* x, std::string const& name)
            {
                expect<std::string>(ist, x, name);
            }

            #define EXPECT(WHAT) expect(ist, WHAT, name)
        }

        inline namespace parse_details
        {
            inline namespace impl
            {
                template <class T>
                T parse(std::istream & ist, std::string const& name)
                {
                    T x;
                    if (ist >> x) return x;
                    throw std::runtime_error{"failed to parse " + name};
                }

                #define PARSE_FOR(WHAT) inline namespace impl
                #define PARSE(TYPE, NAME) parse<TYPE>(ist, name + ": " #NAME)
                #define PARSE_KV(TYPE, NAME) (EXPECT(#NAME), PARSE(TYPE, NAME))

                #define FN_PARSE(TYPE) \
                    template <> \
                    TYPE parse<TYPE>(std::istream & ist, std::string const& name)


                #define RETURN_PARSE_BLOCK(TYPE, CONTENT...) do { \
                    EXPECT("{"); \
                    TYPE parse_block__item \
                        CONTENT; \
                    EXPECT("}"); \
                    return parse_block__item; \
                } while (false)

                #define FN_PARSE_BLOCK(TYPE, CONTENT...) \
                    FN_PARSE(TYPE) \
                    { \
                        RETURN_PARSE_BLOCK(TYPE, CONTENT); \
                    }


                #define RETURN_PARSE_VARIANT(NAME, CONTENT...) do { \
                    auto parse_variant__kind = PARSE(std::string, NAME-kind); \
                    CONTENT; \
                    throw std::runtime_error{"Unknown " #NAME "-kind: " + parse_variant__kind}; \
                } while (false)

                #define RETURN_PARSE_VARIANT_ALTERNATIVE(TYPE, NAME) \
                    if (parse_variant__kind == #NAME) return PARSE(TYPE, NAME);

                #define FN_PARSE_VARIANT(TYPE, NAME, CONTENT...) \
                    FN_PARSE(TYPE) \
                    { \
                        RETURN_PARSE_VARIANT(NAME, CONTENT); \
                    }


                #define RETURN_PARSE_VARIANT_LIST(TYPE, NAME, CONTENT...) do { \
                    EXPECT("{"); \
                    TYPE parse_variant_list__container; \
                    while (true) { \
                        auto parse_variant_list__kind = PARSE(std::string, NAME-kind); \
                        if (parse_variant_list__kind == "}") { \
                            return parse_variant_list__container; \
                        } \
                        CONTENT; \
                        throw std::runtime_error{"Unknown " #NAME "-kind: " + parse_variant_list__kind}; \
                    } \
                } while (false)

                #define RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(TYPE, NAME) \
                    if (parse_variant_list__kind == #NAME) { \
                        parse_variant_list__container.emplace_back(PARSE(TYPE, NAME)); \
                        continue; \
                    }

                #define FN_PARSE_VARIANT_LIST(TYPE, NAME, CONTENT...) \
                    FN_PARSE(TYPE) \
                    { \
                        RETURN_PARSE_VARIANT_LIST(TYPE, NAME, CONTENT); \
                    }
            }

            PARSE_FOR(primitives)
            {
                FN_PARSE(std::string)
                {
                    std::string x;
                    if (ist >> std::quoted(x)) return x;
                    throw std::runtime_error{"failed to parse " + name};
                }

                FN_PARSE(glm::ivec2)
                {
                    return {
                        PARSE(int, ivec2.x),
                        PARSE(int, ivec2.y),
                    };
                }

                FN_PARSE(glm::vec2)
                {
                    return {
                        PARSE(float, vec2.x),
                        PARSE(float, vec2.y),
                    };
                }

                FN_PARSE(glm::vec3)
                {
                    return {
                        PARSE(float, vec3.x),
                        PARSE(float, vec3.y),
                        PARSE(float, vec3.z),
                    };
                }

                FN_PARSE(glm::vec4)
                {
                    return {
                        PARSE(float, vec4.x),
                        PARSE(float, vec4.y),
                        PARSE(float, vec4.z),
                        PARSE(float, vec4.w),
                    };
                }

                FN_PARSE(glm::mat4)
                {
                    return {
                        PARSE(glm::vec4, mat4.x),
                        PARSE(glm::vec4, mat4.y),
                        PARSE(glm::vec4, mat4.z),
                        PARSE(glm::vec4, mat4.w),
                    };
                }
            }

            PARSE_FOR(background)
            {
                struct background
                {
                    glm::vec3 color;
                };

                FN_PARSE_BLOCK(background, {
                    ([&] () {
                        auto color = PARSE_KV(glm::vec3, color);
                        auto strength = PARSE_KV(float, strength);
                        return color * strength;
                    })(),
                });
            }

            PARSE_FOR(lamp_list)
            {
                FN_PARSE_BLOCK(lamps::sun, {
                    PARSE_KV(glm::vec3, direction),
                    ([&] () {
                        auto color = PARSE_KV(glm::vec3, color);
                        auto strength = PARSE_KV(float, strength);
                        return color * strength;
                    })(),
                });

                FN_PARSE_BLOCK(lamps::omni, {
                    PARSE_KV(glm::vec3, center),
                    ([&] () {
                        auto color = PARSE_KV(glm::vec3, color);
                        auto strength = PARSE_KV(float, strength);
                        return color * strength;
                    })(),
                });

                FN_PARSE_VARIANT_LIST(lamp_container_type, lights, {
                    RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(lamps::sun, directional-light);
                    RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(lamps::omni, point-light);
                });
            }

            PARSE_FOR(camera)
            {
                FN_PARSE_BLOCK(cameras::orthographic, {
                    PARSE_KV(glm::vec3, center),
                    PARSE_KV(glm::vec3, direction),
                    PARSE_KV(glm::vec3, up),
                    PARSE_KV(float, size),
                });

                FN_PARSE_BLOCK(cameras::pin_hole, {
                    PARSE_KV(glm::vec3, center),
                    PARSE_KV(glm::vec3, direction),
                    PARSE_KV(glm::vec3, up),
                    glm::radians(PARSE_KV(float, angle)),
                });

                FN_PARSE_VARIANT(camera_type, camera, {
                    RETURN_PARSE_VARIANT_ALTERNATIVE(cameras::orthographic, orthographic-camera);
                    RETURN_PARSE_VARIANT_ALTERNATIVE(cameras::pin_hole, perspective-camera);
                });
            }

            PARSE_FOR(view)
            {
                FN_PARSE_BLOCK(view_type, {
                    PARSE_KV(std::string, name),
                    PARSE_KV(glm::ivec2, dimension),
                    PARSE_KV(int, bounces),
                    PARSE_KV(int, samples),
                    PARSE(camera_type, camera),
                });

                FN_PARSE_VARIANT_LIST(view_container_type, views, {
                    RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(view_type, view);
                });
            }

            PARSE_FOR(material_list)
            {
                FN_PARSE_BLOCK(materials::phong, {
                    PARSE_KV(glm::vec3, diffuse-color),
                    PARSE_KV(glm::vec3, reflection-color),
                    PARSE_KV(glm::vec3, refraction-color),
                    PARSE_KV(glm::vec3, specular-color),
                    PARSE_KV(float, exponent),
                    PARSE_KV(float, index-of-refraction),
                    PARSE_KV(float, roughness),
                });

                FN_PARSE(texture_packs::pure)
                {
                    return {};
                }

                FN_PARSE_BLOCK(texture_packs::checkerboard, {
                    PARSE_KV(glm::vec3, accent),
                    PARSE_KV(float, size),
                });

                FN_PARSE_BLOCK(texture_packs::pbr_checkerboard, {
                    PARSE_KV(glm::vec3, albedo-accent),
                    PARSE_KV(float, albedo-size),

                    PARSE_KV(float, roughness-accent),
                    PARSE_KV(float, roughness-size),

                    PARSE_KV(float, density-accent),
                    PARSE_KV(float, density-size),
                });

                FN_PARSE_BLOCK(texture_packs::noise_fbm, {
                    PARSE_KV(glm::vec3, albedo-accent),
                    PARSE_KV(int, albedo-details),
                    PARSE_KV(float, albedo-size),
                    PARSE_KV(glm::vec3, albedo-seed),
                    PARSE_KV(glm::vec2, albedo-range),

                    PARSE_KV(float, roughness-accent),
                    PARSE_KV(int, roughness-details),
                    PARSE_KV(float, roughness-size),
                    PARSE_KV(glm::vec3, roughness-seed),
                    PARSE_KV(glm::vec2, roughness-range),

                    PARSE_KV(float, density-accent),
                    PARSE_KV(int, density-details),
                    PARSE_KV(float, density-size),
                    PARSE_KV(glm::vec3, density-seed),
                    PARSE_KV(glm::vec2, density-range),
                });

                FN_PARSE_VARIANT(texture_pack_type, texture-pack, {
                    RETURN_PARSE_VARIANT_ALTERNATIVE(texture_packs::pure, pure);
                    RETURN_PARSE_VARIANT_ALTERNATIVE(texture_packs::checkerboard, checkerboard);
                    RETURN_PARSE_VARIANT_ALTERNATIVE(texture_packs::pbr_checkerboard, pbr-checkerboard);
                    RETURN_PARSE_VARIANT_ALTERNATIVE(texture_packs::noise_fbm, noise-fbm);
                });

                struct pbr_material
                {
                    texture_pack_type texture_pack;
                    glm::vec3 color;
                    float roughness;
                    float metalness;
                    float ior;

                    operator material_type ()
                    {
                        return materials::physically_based{
                            std::move(texture_pack),
                            color,
                            metalness,
                            roughness,
                            ior,
                        };
                    }
                };

                FN_PARSE_BLOCK(pbr_material, {
                    PARSE(texture_pack_type, texture-pack),
                    PARSE_KV(glm::vec3, color),
                    PARSE_KV(float, roughness),
                    PARSE_KV(float, metalness),
                    PARSE_KV(float, index-of-refraction),
                });

                struct microfacet_bsdf
                {
                    texture_pack_type texture_pack;
                    glm::vec3 base;
                    glm::vec3 emission;
                    float metalness;
                    float roughness;
                    float ior;
                    float opacity;
                    float density;

                    operator material_type ()
                    {
                        return materials::physically_based{
                            std::move(texture_pack),
                            base,
                            emission,
                            metalness,
                            roughness,
                            ior,
                            opacity,
                            density,
                        };
                    }
                };

                FN_PARSE_BLOCK(microfacet_bsdf, {
                    PARSE(texture_pack_type, texture-pack),
                    PARSE_KV(glm::vec3, base),
                    PARSE_KV(glm::vec3, emission),
                    PARSE_KV(float, metalness),
                    PARSE_KV(float, roughness),
                    PARSE_KV(float, ior),
                    PARSE_KV(float, opacity),
                    PARSE_KV(float, density),
                });

                FN_PARSE_VARIANT_LIST(material_container_type, material, {
                    RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(materials::phong, phong-material);
                    RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(pbr_material, pbr-material);
                    RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(microfacet_bsdf, microfacet-bsdf);
                });
            }

            PARSE_FOR(shape)
            {
                FN_PARSE_BLOCK(shapes::sphere, {
                    PARSE_KV(glm::vec3, center),
                    PARSE_KV(float, radius),
                });

                FN_PARSE_BLOCK(shapes::plane, {
                    PARSE_KV(glm::vec3, normal),
                    PARSE_KV(float, offset),
                });

                FN_PARSE(shapes::mesh)
                {
                    auto path = PARSE(std::string, path);
                    return mesh_loader::from_path(path);
                }

                struct voxel_metadata
                {
                    shapes::mesh mesh;
                    int subdivision;
                };

                FN_PARSE_BLOCK(voxel_metadata, {
                    PARSE_KV(shapes::mesh, mesh),
                    PARSE_KV(int, subdivision),
                });

                FN_PARSE(shapes::voxel)
                {
                    using mesh_face_trait = raytracer::face_trait::mesh;
                    using mesh_grid_type = tool::grid<mesh_face_trait>;

                    auto md = PARSE(voxel_metadata, voxel);
                    auto& m = md.mesh;

                    mesh_grid_type::face_soup_type face_ids;
                    auto face_count = static_cast<int>(m.faces.size());
                    face_ids.reserve(face_count);
                    for (int i=0; i<face_count; i++)
                        face_ids.emplace_back(i);

                    mesh_grid_type grid{mesh_face_trait{m}, md.subdivision, std::move(face_ids)};
                    return {
                        std::move(m),
                        std::move(grid),
                    };
                }

                FN_PARSE_VARIANT(shape_type, shape, {
                    RETURN_PARSE_VARIANT_ALTERNATIVE(shapes::sphere, sphere);
                    RETURN_PARSE_VARIANT_ALTERNATIVE(shapes::plane, plane);
                    RETURN_PARSE_VARIANT_ALTERNATIVE(shapes::mesh, mesh);
                    RETURN_PARSE_VARIANT_ALTERNATIVE(shapes::voxel, voxel);
                });
            }

            PARSE_FOR(node)
            {
                FN_PARSE_BLOCK(nodes::object, {
                    PARSE_KV(material_id_type, material-id),
                    PARSE(shape_type, shape),
                });

                FN_PARSE(nodes::group);
                FN_PARSE(nodes::xform);

                FN_PARSE_VARIANT_LIST(group_node_container_type, node, {
                    RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(nodes::group, group);
                    RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(nodes::xform, xform);
                    RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(nodes::object, object);
                });

                FN_PARSE_VARIANT(node_type, node, {
                    RETURN_PARSE_VARIANT_ALTERNATIVE(nodes::group, group);
                    RETURN_PARSE_VARIANT_ALTERNATIVE(nodes::xform, xform);
                    RETURN_PARSE_VARIANT_ALTERNATIVE(nodes::object, object);
                });

                FN_PARSE(nodes::group)
                {
                    return {
                        PARSE(group_node_container_type, nodes),
                    };
                }

                FN_PARSE_BLOCK(nodes::xform, {
                    PARSE_KV(glm::mat4, model),
                    PARSE(node_type, node),
                });
            }

            PARSE_FOR(scene)
            {
                FN_PARSE(scene_type)
                {
                    auto scene_name = PARSE_KV(std::string, name);
                    auto views = PARSE_KV(view_container_type, views);
                    auto lamps = PARSE_KV(lamp_container_type, lights);
                    auto bg = PARSE_KV(background, background);
                    auto mats = PARSE_KV(material_container_type, materials);
                    auto node = PARSE_KV(nodes::group, group);

                    mats.emplace_back(materials::solid_color{bg.color});
                    auto env_id = mats.size() - 1;

                    return {
                        std::move(scene_name),
                        std::move(views),
                        std::move(lamps),
                        std::move(mats),
                        std::move(node),
                        std::move(env_id),
                    };
                }
            }
        }
    }

    scene_type from_istream(std::istream & ist)
    {
        auto scene = parse<scene_type>(ist, "scene");
        scene.rebuild_cache();
        return scene;
    }

    scene_type from_path(tool::as_czstring path)
    {
        std::ifstream ifs{path.data()};
        return from_istream(ifs);
    }
}

