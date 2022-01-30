#include "../../lib/std/any.hh"
#include "../../util/span.hh"
#include "../../kul/timeline.hh"
#include "../op.hh"
#include "util.hh"
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
}

