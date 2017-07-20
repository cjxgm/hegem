#include "../lib/glm/vec2.hh"
#include "../lib/glm/vec3.hh"
#include "../lib/glm/vec4.hh"
#include "../lib/glm/mat4.hh"
#include "../lib/glm/op/trig.hh"
#include "../lib/glm/op/common.hh"
#include "../lib/glm/op/geom.hh"
#include "parser.hh"
#include "camera.hh"
#include "view.hh"
#include "material.hh"
#include "shape.hh"
#include "node.hh"
#include "lamp.hh"
#include "mesh-loader.hh"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace rt::scene
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
                });

                struct pbr_material
                {
                    glm::vec3 color;
                    float roughness;
                    float metalness;
                    float ior;
                };

                FN_PARSE_BLOCK(pbr_material, {
                    PARSE_KV(glm::vec3, color),
                    PARSE_KV(float, roughness),
                    PARSE_KV(float, metalness),
                    PARSE_KV(float, index-of-refraction),
                });

                FN_PARSE(materials::physically_based)
                {
                    auto pbr = PARSE(pbr_material, pbr);
                    return {
                        mix(pbr.color, glm::vec3{0, 0, 0}, pbr.metalness),
                        mix(glm::vec3{1, 1, 1}, pbr.color, pbr.metalness),
                        pbr.roughness,
                        pbr.ior,
                    };
                }

                FN_PARSE_VARIANT_LIST(material_container_type, material, {
                    RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(materials::phong, phong-material);
                    RETURN_PARSE_VARIANT_LIST_ALTERNATIVE(materials::physically_based, pbr-material);
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

                FN_PARSE_VARIANT(shape_type, shape, {
                    RETURN_PARSE_VARIANT_ALTERNATIVE(shapes::sphere, sphere);
                    RETURN_PARSE_VARIANT_ALTERNATIVE(shapes::plane, plane);
                    RETURN_PARSE_VARIANT_ALTERNATIVE(shapes::mesh, mesh);
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

    scene_type from_path(util::as_czstring path)
    {
        std::ifstream ifs{path.data()};
        return from_istream(ifs);
    }
}

