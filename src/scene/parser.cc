#include "../lib/glm/vec3.hh"
#include "parser.hh"
#include "camera.hh"
#include "view.hh"
#include "material.hh"
#include "shape.hh"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace rt::scene
{
    namespace
    {
        using material_container_type = scene_type::material_container_type;

        template <class T>
        auto all_to_string(T&& x)
        {
            std::ostringstream oss;
            oss << x;
            return oss.str();
        }

        template <class T>
        T expect(std::string const& name, std::istream & ist)
        {
            T x;
            if (ist >> x) return x;
            throw std::runtime_error{name + " expected"};
        }

        template <class T>
        void expect(std::istream & ist, T&& x)
        {
            T v;
            if ((ist >> v) && v == x) return;
            throw std::runtime_error{"\"" + all_to_string(x) + "\" literal expected"};
        }

        void expect(std::istream & ist, char const* x)
        {
            expect<std::string>(ist, x);
        }

        template <>
        glm::vec3 expect<glm::vec3>(std::string const& name, std::istream & ist)
        {
            return {
                expect<float>(name + ": vec3.x", ist),
                expect<float>(name + ": vec3.y", ist),
                expect<float>(name + ": vec3.z", ist),
            };
        }

        struct background
        {
            glm::vec3 color;
        };

        template <>
        background expect<background>(std::string const& name, std::istream & ist)
        {
            expect(ist, "background");
            expect(ist, "{");
            background bg{
                (expect(ist, "color"), expect<glm::vec3>(name + ": color", ist)),
            };
            expect(ist, "}");
            return bg;
        }

        template <>
        camera_type expect<camera_type>(std::string const& name, std::istream & ist)
        {
            auto kind = expect<std::string>(name + ": camera kind", ist);

            if (kind == "orthographic-camera") {
                expect(ist, "{");
                cameras::orthographic cam{
                    (expect(ist, "center"), expect<glm::vec3>(name + ": center", ist)),
                    (expect(ist, "direction"), expect<glm::vec3>(name + ": direction", ist)),
                    (expect(ist, "up"), expect<glm::vec3>(name + ": up", ist)),
                    (expect(ist, "size"), expect<float>(name + ": size", ist)),
                };
                expect(ist, "}");
                return cam;
            }

            throw std::runtime_error{"Unknown camera kind: " + kind};
        }

        template <>
        materials::phong expect<materials::phong>(std::string const& name, std::istream & ist)
        {
            expect(ist, "{");
            materials::phong mat{
                (expect(ist, "diffuse-color"), expect<glm::vec3>(name + ": diffuse color", ist)),
            };
            expect(ist, "}");
            return mat;
        }

        template <>
        material_container_type expect<material_container_type>(std::string const& name, std::istream & ist)
        {
            material_container_type mats;
            expect(ist, "materials");
            expect(ist, "{");

            while (true) {
                auto kind = expect<std::string>(name + ": material kind", ist);

                if (kind == "}") {
                    return mats;
                }

                if (kind == "phong-material") {
                    mats.emplace_back(expect<materials::phong>(name + ": phong", ist));
                    continue;
                }

                throw std::runtime_error{"Unknown material kind: " + kind};
            }
        }

        template <>
        shapes::sphere expect<shapes::sphere>(std::string const& name, std::istream & ist)
        {
            expect(ist, "{");
            shapes::sphere geo{
                (expect(ist, "center"), expect<glm::vec3>(name + ": center", ist)),
                (expect(ist, "radius"), expect<float>(name + ": radius", ist)),
            };
            expect(ist, "}");
            return geo;
        }

        template <>
        shape_type expect<shape_type>(std::string const& name, std::istream & ist)
        {
            auto kind = expect<std::string>(name + ": shape kind", ist);

            if (kind == "sphere") {
                return expect<shapes::sphere>(name + ": sphere", ist);
            }

            throw std::runtime_error{"Unknown shape kind: " + kind};
        }

        template <>
        nodes::object expect<nodes::object>(std::string const& name, std::istream & ist)
        {
            expect(ist, "{");
            nodes::object node{
                (expect(ist, "material-id"), expect<material_id_type>(name + ": material identity", ist)),
                expect<shape_type>(name + ": shape", ist),
            };
            expect(ist, "}");
            return node;
        }

        template <>
        nodes::group expect<nodes::group>(std::string const& name, std::istream & ist)
        {
            expect(ist, "{");
            nodes::group group;

            while (true) {
                auto kind = expect<std::string>(name + ": node kind", ist);

                if (kind == "}") {
                    return group;
                }

                if (kind == "group") {
                    group.nodes.emplace_back(expect<nodes::group>(name + ": group", ist));
                    continue;
                }

                if (kind == "object") {
                    group.nodes.emplace_back(expect<nodes::object>(name + ": object", ist));
                    continue;
                }

                throw std::runtime_error{"Unknown node kind: " + kind};
            }
        }

        template <>
        node_type expect<node_type>(std::string const& name, std::istream & ist)
        {
            expect(ist, "group");
            return expect<nodes::group>(name + ": group node", ist);
        }
    }

    scene_type from_istream(std::istream & ist)
    {
        auto cam = expect<camera_type>("camera", ist);
        auto bg = expect<background>("background", ist);
        auto mats = expect<material_container_type>("materials", ist);
        auto node = expect<node_type>("root node", ist);

        mats.emplace_back(materials::solid_color{bg.color});
        auto env_id = mats.size() - 1;
        return {
            std::move(mats),
            { view_type{{}, cam} },
            {},     // TODO: parse lamps
            env_id,
            node,
        };
    }

    scene_type from_path(gsl::cstring_span<> path)
    {
        std::ifstream ifs{ensure_z(path).data()};
        return from_istream(ifs);
    }
}

