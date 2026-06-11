#include "scheduler.hxx"

namespace hegem::util
{
    pool_scheduler::pool_scheduler(int thread_count)
        : pool{std::make_unique<cxxpool::thread_pool>(thread_count)}
    {}

    void pool_scheduler::push(std::function<void()> f)
    {
        pool->push(f);
    }
}

