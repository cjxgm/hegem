#include "op.hxx"
#include "../util/unreachable.macro.hxx"

namespace hegem::skein
{
    namespace op
    {
        op_instance::op_instance(op_id id)
            : id{id}
        {
            switch (id) {
                #define OP(KIND, ID, ARITY, NAME, TOOLTIP, FIELDS...) \
                    case op_id::KIND##_##ID: { \
                        auto& fields = this->fields.KIND##_##ID; \
                        (void)fields; \
                        FIELDS \
                    } break;
                #define FIELD(TYPE, VAR, INITIAL, ...) fields.VAR = INITIAL;
                #include "op.inl"

                default: RT_UNREACHABLE();
            }
        }
    }
}

