#pragma once
#include "../lib/std/any.hxx"
#include "graph.hxx"

namespace hegem::skein
{
    namespace engine
    {
        auto sanity_check(graph& g0) -> void;
        auto execute(graph& g, node* entry) -> lib::any;
    }
}

