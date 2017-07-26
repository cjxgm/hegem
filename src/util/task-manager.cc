#include "task-manager.hh"
#include <iostream>
#include <stdexcept>

namespace rt::util
{
    auto spawner::spawn(std::function<void()> f) -> task_id_type
    {
        task_id_type id;
        pool.push([f=std::move(f), canceling=id.canceling.get(), dead=id.dead.get()] () {
            if (canceling->load()) {
                dead->store(true);
                return;
            }

            bool err = false;
            try {
                f();
            }
            catch (std::exception const& ex) {
                std::cerr << ex.what() << "\n";
                err = true;
            }
            catch (...) {
                err = true;
            }

            dead->store(true);
            if (err) std::abort();
        });
        return id;
    }

    void spawner::cancel(task_id_type& id)
    {
        id.canceling->store(true);
    }

    void task_manager::spawn(std::string tag, std::function<void()> f)
    {
        auto id = sp.spawn(std::move(f));
        tasks.push_back({
            std::move(tag),
            std::move(id),
        });
    }

    void task_manager::cancel(std::string const& tag)
    {
        for (auto& task: tasks)
            if (task.tag == tag)
                sp.cancel(task.id);
    }

    void task_manager::gc()
    {
        auto first = begin(tasks);
        auto last = end(tasks);
        tasks.erase(std::remove_if(first, last, [] (auto& task) {
            return task.id.is_dead();
        }), last);
    }
}

