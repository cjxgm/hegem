#pragma once
#include "../lib/cxxpool.hh"
#include <functional>
#include <atomic>
#include <vector>
#include <string>

namespace rt::util
{
    struct task_identity
    {
        task_identity()
            : canceling{std::make_unique<std::atomic_bool>()}
            , dead{std::make_unique<std::atomic_bool>()}
        {}
        bool is_dead() const { return dead->load(); }

    private:
        std::unique_ptr<std::atomic_bool> canceling;
        std::unique_ptr<std::atomic_bool> dead;

        friend struct spawner;
    };

    struct spawner
    {
        using task_id_type = task_identity;

        spawner(int thread_count) : pool(thread_count) {}
        task_id_type spawn(std::function<void()> f);
        void cancel(task_id_type& id);

    private:
        cxxpool::thread_pool pool;
    };

    struct tagged_task
    {
        std::string tag;
        task_identity id;
    };

    struct task_manager
    {
        task_manager(int thread_count): sp{thread_count} {}
        void spawn(std::string tag, std::function<void()> f);   // can only be called on GL thread
        void cancel(std::string const& tag);
        void gc();      // should be called every frame

    private:
        spawner sp;
        std::vector<tagged_task> tasks;
    };
}

