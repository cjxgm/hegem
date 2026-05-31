#pragma once
#include "../lib/cxxpool.hh"
#include <functional>
#include <memory>

namespace rt::util
{
    struct pool_scheduler
    {
        pool_scheduler(int thread_count);
        void push(std::function<void()> f);

    private:
        std::unique_ptr<cxxpool::thread_pool> pool;
    };
}

