#include "../../lib/std/any.hh"
#include "../../util/span.hh"
#include "../op.hh"
#include "util.hh"
#include <utility>      // for std::move
#include <memory>
#include <vector>
#include <stdexcept>

namespace rt::sk::op::invoke_impl
{
    namespace
    {
        using pack_content_type = std::vector<lib::any>;
        using pack_type = std::shared_ptr<pack_content_type>;
    }

    auto invoke(op_fields_indirection_nop const& fields, util::span<lib::any> args) -> lib::any
    {
        return std::move(args[0]);
    }

    auto invoke(op_fields_indirection_pack const& fields, util::span<lib::any> args) -> lib::any
    {
        pack_content_type content;
        for (auto& arg: args.range())
            content.emplace_back(std::move(arg));
        return std::make_shared<decltype(content)>(std::move(content));
    }

    auto invoke(op_fields_indirection_unpack const& fields, util::span<lib::any> args) -> lib::any
    {
        auto pack = extract_or_croak<pack_type>(args[0], "Argument must be a pack.");
        auto& content = *pack;

        if (fields.index < 0 || fields.index >= int(content.size()))
            throw std::runtime_error{"Index out of range."};

        auto& value = content[fields.index];
        if (value.type() == typeid(void))
            throw std::runtime_error{"Value has previously been extracted exclusively."};

        if (fields.exclusive) {
            return std::move(value);
        } else {
            return value;
        }
    }
}

