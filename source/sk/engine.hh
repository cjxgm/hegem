#pragma once
#include "../lib/std/any.hh"
#include "graph.hh"

namespace rt::sk
{
    namespace engine
    {
        void sanity_check(graph& g0);
        auto execute(graph& g, node* entry) -> lib::any;
    }
}

