#include "scheduler.hxx"

namespace hegem::tool
{
    pool_scheduler::pool_scheduler(int thread_count)
        : pool{std::make_unique<cxxpool::thread_pool>(thread_count)}
    {}

    auto pool_scheduler::push(std::function<void()> f) -> void
    {
        pool->push(f);
    }
}

