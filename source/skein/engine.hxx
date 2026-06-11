#pragma once
#include "../lib/std/any.hxx"
#include "graph.hxx"

namespace hegem::skein
{
    namespace engine
    {
        void sanity_check(graph& g0);
        auto execute(graph& g, node* entry) -> lib::any;
    }
}

