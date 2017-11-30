#include "invoke.hh"
#include "invoke.impl/all.pull"

namespace rt::sk
{
    namespace op
    {
        namespace invoke_impl
        {
            #define OP(KIND, ID, ...) \
                auto invoke(op_fields_##KIND##_##ID const& fields, util::span<lib::any> args) -> lib::any;
            #include "op.inl"
        }

        auto invoke(op_instance const& instance, util::span<lib::any> arguments) -> lib::any
        {
            switch (instance.id) {
                #define OP(KIND, ID, ...) \
                    case op_id::KIND##_##ID: \
                        return invoke_impl::invoke(instance.fields.KIND##_##ID, arguments);
                #include "op.inl"
            }
            return {};
        }
    }
}

