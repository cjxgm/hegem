#pragma once
#include "../lib/cxxpool.hh"
#include <functional>

namespace rt::util
{
    // Fire-and-forget threaded tasks spawner
    struct spawner
    {
        spawner(int thread_count) : pool(thread_count) {}
        void spawn(std::function<void()> f);

    private:
        cxxpool::thread_pool pool;
    };
}

