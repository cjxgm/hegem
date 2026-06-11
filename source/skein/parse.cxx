#include "../lib/cpptoml.hxx"
#include "../tool/journal.hxx"
#include "../tool/unreachable.macro.hxx"
#include "parse.hxx"
#include <stdexcept>
#include <array>
#include <algorithm>
#include <type_traits>
#include <typeinfo>

namespace hegem::skein
{
    namespace
    {
        tool::journal j() { return {"SK.PARSE"}; }

        auto op_id_from_name(graph& g, std::string const& name)
        {
            for (auto& entry: g.op_metadata_range())
                if (entry.second.id == name)
                    return entry.first;
            throw std::runtime_error{"Unknown operator: " + name};
        }

        using op::float3;
    }

    namespace parse_toml
    {
        namespace
        {
            namespace get_or_die_details
            {
                template <class T>
                constexpr auto is_genuine_integral_v = std::is_integral_v<T> && !std::is_same_v<T, bool>;

                template <class T>
                using intermediate_type_of = std::conditional_t<
                    is_genuine_integral_v<T>, int64_t,
                    std::conditional_t<
                        std::is_floating_point_v<T>, double,
                        T
                    >
                >;

                template <class T>
                using journal_type_of = std::conditional_t<
                    is_genuine_integral_v<T>, int,
                    std::conditional_t<
                        std::is_floating_point_v<T>, float,
                        T
                    >
                >;

                template <class T>
                struct impl
                {
                    using intermediate_type = intermediate_type_of<T>;

                    static auto get(cpptoml::table const& tbl, std::string const& key)
                    {
                        auto opt_value = tbl.get_as<intermediate_type>(key);
                        j()
                            << "VALUE("
                            << typeid(intermediate_type).name()
                            << "->"
                            << typeid(T).name()
                            << ") "
                            << key
                            << " = "
                            << (opt_value ? "" : "nil ")
                            << journal_type_of<T>(*opt_value)
                            << "\n";
                        if (!opt_value) throw std::runtime_error{"Expect value: " + key};
                        return T(std::move(*opt_value));
                    }
                };

                template <class T, size_t N>
                struct impl<std::array<T, N>>
                {
                    using result_type = std::array<T, N>;
                    using intermediate_type = intermediate_type_of<T>;

                    static auto get(cpptoml::table const& tbl, std::string const& key)
                    {
                        auto opt_arr = tbl.get_array_of<intermediate_type>(key);
                        if (!opt_arr) throw std::runtime_error{"Expect array: " + key};
                        auto& arr = *opt_arr;

                        if (arr.size() != N) {
                            throw std::runtime_error{
                                "Expect array "
                                + key
                                + " of size "
                                + std::to_string(N)
                                + ", but got an array of size "
                                + std::to_string(arr.size())
                            };
                        }

                        j() << "ARRAY " << key << "[" << int(arr.size()) << "]:\n";
                        for (auto x: arr) j() << "- " << journal_type_of<T>(x) << "\n";

                        result_type result;
                        if constexpr (std::is_same_v<T, intermediate_type>) {
                            std::move(begin(arr), end(arr), begin(result));
                        } else {
                            std::transform(begin(arr), end(arr), begin(result), [] (auto x) { return T(x); });
                        }
                        return result;
                    }
                };

                template <>
                struct impl<cpptoml::table>
                {
                    static auto get(cpptoml::table const& tbl, std::string const& key)
                    {
                        auto shared_table = tbl.get_table(key);
                        if (!shared_table) throw std::runtime_error{"Expect table: " + key};
                        j() << "TABLE " << key << ":\n";
                        for (auto& e: *shared_table)
                            j() << "* " << e.first << "\n";
                        return shared_table;
                    }
                };

                template <class T>
                auto get_or_die(cpptoml::table const& tbl, std::string const& key)
                {
                    return impl<T>::get(tbl, key);
                }
            }
            using get_or_die_details::get_or_die;
        }

        namespace read
        {
            namespace value_details
            {
                template <class T>
                struct impl
                {
                    static auto get(cpptoml::table const& tbl, std::string const& key)
                    {
                        return get_or_die<T>(tbl, key);
                    }
                };

                template <class T>
                auto value(cpptoml::table const& tbl, std::string const& key)
                {
                    return impl<T>::get(tbl, key);
                }
            }
            using value_details::value;
        }
    }

    auto parse(char const* path) -> std::tuple<graph, node_id_type>
    {
        graph g;
        node_id_type previewing_node{};

        try {
            auto config = cpptoml::parse_file(path);
            auto cfg_nodes = config->get_table_array("node");
            if (cfg_nodes) {
                for (auto cfg_node: *cfg_nodes) {
                    auto pos = parse_toml::get_or_die<std::array<int, 2>>(*cfg_node, "pos");
                    auto width = parse_toml::get_or_die<int>(*cfg_node, "width");
                    auto opt_preview = cfg_node->get_as<bool>("preview");

                    auto cfg_instance_table = parse_toml::get_or_die<cpptoml::table>(*cfg_node, "instance");
                    if (cfg_instance_table->empty() || ++cfg_instance_table->begin() != cfg_instance_table->end()) {
                        throw std::runtime_error{
                            "node.instance should contain exactly 1 sub-table"
                        };
                    }
                    auto& op_name = cfg_instance_table->begin()->first;
                    auto id = op_id_from_name(g, op_name);

                    auto& node = g.emplace(pos[0], pos[1], width, id);
                    if (opt_preview && *opt_preview) previewing_node = node.id;

                    auto cfg_instance = parse_toml::get_or_die<cpptoml::table>(*cfg_instance_table, op_name);
                    auto& instance = node.instance;
                    switch (instance.id) {
                        #define OP(KIND, ID, ARITY, NAME, TOOLTIP, FIELDS...) \
                            case op_id::KIND##_##ID: { \
                                auto& fields = instance.fields.KIND##_##ID; \
                                (void)fields; \
                                FIELDS \
                            } break;
                        #define SECTION(ID, ...) \
                            case op_id::section_##ID##_##ID: RT_UNREACHABLE();
                        #define FIELD(TYPE, VAR, INITIAL, WIDGET, ...) \
                            fields.VAR = parse_toml::read::WIDGET<TYPE>(*cfg_instance, #VAR);
                        #include "op.inl"
                    }
                }
            }
        }
        catch (std::runtime_error const& e) {
            throw std::runtime_error{
                std::string{"Failed to parse "}
                + path
                + ": "
                + e.what()
            };
        }

        return {
            std::move(g),
            previewing_node,
        };
    }
}

