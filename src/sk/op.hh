#pragma once

namespace rt::sk
{
    namespace op
    {
        enum class kind_id
        {
            #define KIND(KIND, ...) KIND,
            #include "op.inl"
        };

        enum class op_id
        {
            #define OP(KIND, ID, ...) KIND##_##ID,
            #include "op.inl"
        };

        #define OP(KIND, ID, ARITY, NAME, TOOLTIP, FIELDS...) \
            struct op_fields_##KIND##_##ID \
            { \
                FIELDS \
            };
        #define FIELD(TYPE, VAR, ...) TYPE VAR;
        #include "op.inl"

        struct op_instance
        {
            op_id id;
            union {
                #define OP(KIND, ID, ...) op_fields_##KIND##_##ID KIND##_##ID;
                #include "op.inl"
            } fields;

            op_instance(op_id id);
        };
    }

    using op::kind_id;
    using op::op_id;
    using op::op_instance;
}

