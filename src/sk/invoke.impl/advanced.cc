#include "../../lib/std/any.hh"
#include "../../util/span.hh"
#include "../op.hh"
#include "model.hh"
#include <utility>      // for std::move
#include <stdexcept>

namespace rt::sk::op::invoke_impl
{
    auto invoke(op_fields_advanced_merge const& fields, util::span<lib::any> args) -> lib::any
    {
        model m;
        for (auto& arg: args.range()) {
            if (arg.type() != typeid(model))
                throw std::runtime_error{"Arguments must be models"};
            auto m2 = std::any_cast<model>(std::move(arg));
            m.hmesh.extend(m2.hmesh);
        }
        return m;
    }
}

