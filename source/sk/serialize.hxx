#pragma once
#include "graph.hxx"
#include "../util/unreachable.macro.hxx"

namespace rt::sk
{
    template <class Serializer>
    void serialize(Serializer& s, graph& g, node_id_type previewing_node)
    {
        g.collect_garbage();

        for (auto& n: g.node_range()) {
            auto& instance = n.instance;
            auto previewing = (n.id == previewing_node);

            {
                auto fs = s.node(n.metadata->id, n.x, n.y, n.width, previewing);
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
                        fs.WIDGET(#VAR, fields.VAR);
                    #include "op.inl"
                }
            }
        }
    }
}

#include "../util/unreachable.undef.hxx"

