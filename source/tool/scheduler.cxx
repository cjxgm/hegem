#include "scheduler.hxx"
#include "guard.hxx"
#include "../global/counter.hxx"

namespace hegem::tool
{
    inline namespace
    {
        using global::counter;
    }

    pool_scheduler::pool_scheduler(int thread_count)
        : pool{std::make_unique<cxxpool::thread_pool>(thread_count)}
    {}

    auto pool_scheduler::push(std::function<void()> f) -> void
    {
        counter.task_started.fetch_add(1, std::memory_order::relaxed);  // Tasks are considered "started" when they have reached the scheduler, not when they are going to run.
        pool->push([f = std::move(f)] {
            auto stopped_count_guard = tool::guard{[] {
                counter.task_stopped.fetch_add(1, std::memory_order::relaxed);
            }};
            f();
        });
    }
}

