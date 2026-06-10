#include "../../lib/std/any.hxx"
#include "../../util/span.hxx"
#include "../../kul/timeline.hxx"
#include "../../kul/system.hxx"
#include "../op.hxx"
#include "util.hxx"
#include <utility>      // for std::move
#include <string>
#include <stdexcept>
#include <typeinfo>
#include <cstddef>

namespace rt::sk::op::invoke_impl
{
    auto invoke(op_fields_timeline_system_inspect const& fields, util::span<lib::any> args) -> lib::any
    {
        if (args.size() != 0u) {
            auto result = std::string{};
            auto nth = std::size_t(1);
            for (auto& arg: args.range()) {
                result += "Expr #";
                result += std::to_string(nth);
                if (arg.type() == typeid(kul::timeline)) {
                    auto tl = std::any_cast<kul::timeline>(std::move(arg));
                    result += " = ";
                    result += tl.printable_expression();
                    result += "\n";
                } else {
                    result += " is not a timeline.\n";
                }
                nth++;
            }
            result.pop_back();  // remove the last "\n".

            throw std::runtime_error{result};
        }
        return {};
    }

    auto invoke(op_fields_timeline_system_spark const& fields, util::span<lib::any> args) -> lib::any
    {
        auto tl_pos_x = extract_or_croak<kul::timeline>(args[0], "Argument #1 must be a timeline.");
        auto tl_pos_y = extract_or_croak<kul::timeline>(args[1], "Argument #2 must be a timeline.");
        auto tl_pos_z = extract_or_croak<kul::timeline>(args[2], "Argument #3 must be a timeline.");
        auto tl_radius = extract_or_croak<kul::timeline>(args[3], "Argument #4 must be a timeline.");
        auto tl_emitting_color_r = extract_or_croak<kul::timeline>(args[4], "Argument #5 must be a timeline.");
        auto tl_emitting_color_g = extract_or_croak<kul::timeline>(args[5], "Argument #6 must be a timeline.");
        auto tl_emitting_color_b = extract_or_croak<kul::timeline>(args[6], "Argument #7 must be a timeline.");
        auto tl_opacity = extract_or_croak<kul::timeline>(args[7], "Argument #8 must be a timeline.");

        auto spark = kul::spark_system{};
        spark.pos_x = std::move(tl_pos_x);
        spark.pos_y = std::move(tl_pos_y);
        spark.pos_z = std::move(tl_pos_z);
        spark.radius = std::move(tl_radius);
        spark.emitting_color_r = std::move(tl_emitting_color_r);
        spark.emitting_color_g = std::move(tl_emitting_color_g);
        spark.emitting_color_b = std::move(tl_emitting_color_b);
        spark.opacity = std::move(tl_opacity);
        spark.num_particles = fields.num_particles;

        return std::move(spark);
    }
}

