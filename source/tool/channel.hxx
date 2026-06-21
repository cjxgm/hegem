#pragma once
// An mpsc queue focused on correctness and convenience instead of speed
#include "../lib/std/optional.hxx"
#include "constraints.hxx"
#include <deque>
#include <mutex>
#include <vector>
#include <memory>
#include <atomic>
#include <utility>      // for std::move
#include <functional>
#include <condition_variable>

namespace hegem::tool
{
    namespace channel_details
    {
        template <class Message>
        struct receiver;

        template <class Message>
        struct transmitter;

        template <class Message>
        struct channel_state
        {
            using message_type = Message;

        private:
            std::deque<message_type> pending_messages;
            lib::optional<std::function<void (message_type)>> proxy_fn;
            std::mutex channel_mutex;
            std::condition_variable rx_signaling;

            friend struct receiver<message_type>;
            friend struct transmitter<message_type>;
            template <class M>
            friend auto proxy(receiver<M> rx, std::function<void (M)> fn) -> void;

            auto send(message_type msg) -> void
            {
                auto lock = std::lock_guard{channel_mutex};
                if (proxy_fn) {
                    (*proxy_fn)(std::move(msg));
                } else {
                    pending_messages.emplace_back(std::move(msg));
                    rx_signaling.notify_one();
                }
            }

            lib::optional<message_type> try_recv()
            {
                auto lock = std::lock_guard{channel_mutex};
                if (pending_messages.empty()) return {};
                auto message = pending_messages.front();
                pending_messages.pop_front();
                return { std::move(message) };
            }

            message_type recv()
            {
                auto ulock = std::unique_lock{channel_mutex};
                rx_signaling.wait(ulock, [&] () { return !pending_messages.empty(); });
                auto message = pending_messages.front();
                pending_messages.pop_front();
                return message;
            }
        };

        template <class Message>
        struct transmitter
        {
            using message_type = Message;
            using receiver_type = receiver<message_type>;

            transmitter(receiver_type const& rx)
                : shared_channel{rx.shared_channel}
                , shared_blocked{std::make_shared<std::atomic_bool>()}
            {}

            auto send(message_type msg) -> void
            {
                if (shared_blocked->load()) return;
                shared_channel->send(std::move(msg));
            }

            auto block() -> void { shared_blocked->store(true); }
            bool blocked() const { return shared_blocked->load(); }
            transmitter clone() const { return {shared_channel}; }
            transmitter clone(std::shared_ptr<std::atomic_bool> blocked) const
            {
                return {shared_channel, std::move(blocked)};
            }


        private:
            std::shared_ptr<channel_state<message_type>> shared_channel;
            std::shared_ptr<std::atomic_bool> shared_blocked;

            transmitter(std::shared_ptr<channel_state<message_type>> state)
                : shared_channel{std::move(state)}
                , shared_blocked{std::make_shared<std::atomic_bool>()}
            {}

            transmitter(
                std::shared_ptr<channel_state<message_type>> state,
                std::shared_ptr<std::atomic_bool> blocked
                )
                : shared_channel{std::move(state)}
                , shared_blocked{std::move(blocked)}
            {}
        };

        template <class Message>
        struct receiver: constraints::only_movable
        {
            using message_type = Message;

            lib::optional<message_type> try_recv() { return shared_channel->try_recv(); }
            message_type recv() { return shared_channel->recv(); }
            transmitter<message_type> tx() { return {*this}; }

        private:
            std::shared_ptr<channel_state<message_type>> shared_channel = std::make_shared<channel_state<message_type>>();

            friend struct transmitter<message_type>;
            template <class M>
            friend auto proxy(receiver<M> rx, std::function<void (M)> fn) -> void;
        };

        template <class Message>
        auto proxy(receiver<Message> rx, std::function<void (Message)> fn) -> void
        {
            rx.shared_channel->proxy_fn = fn;
        }

        template <class Message>
        auto bridge(receiver<Message> rx, transmitter<Message> tx) -> void
        {
            proxy<Message>(std::move(rx), [tx = std::move(tx)] (Message message) mutable {
                tx.send(std::move(message));
            });
        }

        template <class Message>
        auto repeat(receiver<Message> rx, std::vector<transmitter<Message>> txs) -> void
        {
            proxy<Message>(std::move(rx), [txs = std::move(txs)] (Message message) mutable {
                for (auto& tx: txs) tx.send(message);
            });
        }
    }

    using channel_details::receiver;
    using channel_details::transmitter;
}

