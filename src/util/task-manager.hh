#pragma once
#include "channel.hh"
#include <memory>
#include <atomic>
#include <vector>
#include <utility>      // for std::move

namespace rt::util
{
    using shared_canceled_type = std::shared_ptr<std::atomic_bool>;

    template <class Message>
    using task_type = std::function<void (transmitter<Message>, shared_canceled_type canceled)>;

    struct task_io
    {
        void cancel() { canceled->store(true); }

    private:
        shared_canceled_type canceled;

        template <class Scheduler>
        friend struct task_manager;

        task_io(
            shared_canceled_type canceled)
            : canceled{std::move(canceled)}
        {}
    };

    template <class Scheduler>
    struct task_manager
    {
        using scheduler_type = Scheduler;

        task_manager(scheduler_type scheduler)
            : scheduler{std::move(scheduler)}
        {}

        template <class Message>
        auto group(transmitter<Message> tx, std::vector<task_type<Message>> tasks) -> task_io
        {
            auto canceled = std::make_shared<std::atomic_bool>(false);

            for (auto t: tasks) {
                scheduler.push([t = std::move(t), tx = tx.clone(canceled), canceled] () mutable {
                    if (canceled->load()) return;
                    t(std::move(tx), std::move(canceled));
                });
            }

            auto io = task_io{std::move(canceled)};
            return io;
        }

        template <class Message>
        auto task(transmitter<Message> tx, task_type<Message> t) -> task_io
        {
            return group(std::move(tx), { std::move(t) });
        }

    private:
        scheduler_type scheduler;
    };
}

