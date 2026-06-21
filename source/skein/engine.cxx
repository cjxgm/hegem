#include "../lib/boost/format.hxx"
#include "../tool/span.hxx"
#include "engine.hxx"
#include "invoke.hxx"
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <stdexcept>
#include <vector>

namespace hegem::skein
{
    namespace engine
    {
        inline namespace
        {
            auto build_output_counts(graph& g, node const* entry)
            {
                std::unordered_map<node const*, int> counts;
                std::deque<node const*> pending{entry};

                while (!pending.empty()) {
                    auto n = pending.front();
                    pending.pop_front();

                    if (counts[n]++) continue;

                    for (auto input: g.inputs_of(n))
                        pending.emplace_back(input);
                }

                return counts;
            }

            auto format_amount(int amount, std::string singular, std::string plural={})
            {
                if (plural.empty()) plural = singular + "s";
                auto num = (amount == 0 ? std::string{"no"} : std::to_string(amount));
                num += " ";
                num += std::move(amount == 1 ? singular : plural);
                return num;
            }

            struct impl
            {
                impl(graph& g, node* entry)
                    : g{g}
                    , entry{entry}
                    , output_counts{build_output_counts(g, entry)}
                {}

                auto operator () () -> lib::any
                {
                    for (auto& node: g.node_range())
                        node.runtime_error.clear();

                    if (execute(entry)) return {};
                    return move_result(entry);
                }

            private:
                graph& g;
                node* entry;
                std::unordered_map<node const*, int> output_counts;
                std::unordered_map<node const*, lib::any> results;

                // UB if results of n does not exist
                auto move_result(node const* n) -> lib::any
                {
                    auto it = results.find(n);
                    if (it == end(results)) {
                        throw std::logic_error{"Accessing non-existent result"};
                    }

                    auto result = std::move(it->second);
                    results.erase(it);

                    return result;
                }

                // Returns false for success
                // Returns true for error
                bool execute(node* n)
                {
                    if (!n->sanity_error.empty()) return true;

                    auto& op = *n->metadata;
                    auto arity = op.arity;

                    auto inputs = g.inputs_of(n);
                    if (arity >= 0 && arity != (int)inputs.size()) {
                        throw std::logic_error{"Arity error should have been reported in sanity check."};
                    }

                    std::vector<lib::any> input_results;
                    if (arity > 0) input_results.reserve(arity);

                    for (auto input: inputs) {
                        auto result_it = results.find(input);
                        if (result_it == end(results)) {
                            if (execute(input)) return true;
                            result_it = results.find(input);
                        }

                        if (--output_counts[input]) {
                            input_results.emplace_back(result_it->second);
                        } else {
                            input_results.emplace_back(std::move(result_it->second));
                            results.erase(result_it);
                        }
                    }

                    try {
                        results.emplace(n, invoke(n->instance, input_results));
                    }
                    catch (std::runtime_error const& e) {
                        n->runtime_error = e.what();
                        return true;
                    }

                    return false;
                }
            };
        }

        auto sanity_check(graph& g) -> void
        {
            for (auto& node: g.node_range()) {
                auto& error = node.sanity_error;
                auto& op = *node.metadata;
                auto& kind = *op.kind;
                auto arity = op.arity;
                auto input_count = g.count_inputs_of(&node);

                error.clear();

                if (arity >= 0 && arity != input_count) {
                    auto fmt = boost::format(
                        "Operator %|| (%||) takes %||, "
                        "but %|| connected.\n"
                    );
                    error += str(fmt
                        % op.name
                        % kind.name
                        % format_amount(arity, "input")
                        % format_amount(input_count, "input is", "inputs are")
                    );
                }

                if (!error.empty() && error.back() == '\n')
                    error.pop_back();
            }
        }

        auto execute(graph& g, node* entry) -> lib::any
        {
            return impl{g, entry}();
        }
    }
}

