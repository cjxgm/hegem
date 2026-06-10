#pragma once
#include "graph.hxx"
#include <tuple>

namespace rt::sk
{
    auto parse(char const* path) -> std::tuple<graph, node_id_type>;
}

