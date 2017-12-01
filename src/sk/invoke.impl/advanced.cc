#include "../../lib/std/any.hh"
#include "../../util/span.hh"
#include "../op.hh"
#include "model.hh"
#include "select.hh"
#include "util.hh"
#include <utility>      // for std::move

namespace rt::sk::op::invoke_impl
{
    auto invoke(op_fields_advanced_merge const& fields, util::span<lib::any> args) -> lib::any
    {
        model m;
        for (auto& arg: args.range()) {
            auto m2 = extract_or_croak<model>(arg, "Arguments must be models.");
            m.hmesh.extend(m2.hmesh);
        }

        select_all(m);

        return m;
    }
}

