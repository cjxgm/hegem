#pragma once
#include "op.hh"
#include "op-registry.hh"

namespace rt::sk
{
    struct node
    {
        int x;
        int y;
        int width;
        op_instance instance;

        // cache
        op_metadata const* metadata;
    };
}

