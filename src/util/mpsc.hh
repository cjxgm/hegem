#pragma once
#include <deque>
#include <mutex>

namespace rt::util
{
    // multi-producer single-consumer queue
    template <
        class T,
        class Mutex=std::mutex,
        template <class> class LockGuard=std::lock_guard
    >
    struct mpsc
    {
        using value_type = T;
        using mutex_type = Mutex;

        template <class GuardMutex>
        using lock_guard = LockGuard<GuardMutex>;

        void push(value_type x)
        {
            lock_guard<mutex_type> _{m};
            queue.emplace_back(std::move(x));
        }

        bool empty() const
        {
            lock_guard<mutex_type> _{m};
            return queue.empty();
        }

        value_type pop()
        {
            lock_guard<mutex_type> _{m};
            auto x = std::move(queue.front());
            queue.pop_front();
            return x;
        }

        operator bool () const { return !empty(); }

    private:
        std::deque<value_type> queue;
        mutable mutex_type m;
    };
}

