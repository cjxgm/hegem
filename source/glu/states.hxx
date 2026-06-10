#pragma once
#include "../lib/gl/gl.hxx"
#include "../util/constraints.hxx"
#include <unordered_set>

namespace rt::glu
{
    struct states_manager: util::non_transferable
    {
        static states_manager& instance();

        // Enable everything in `enabled`, disable anything else.
        // `enabled` must be a subset of `known_toggles`,
        // it's undefined behavior otherwise.
        void enable_only(std::unordered_set<gl::enum_type> enabled);

    private:
        states_manager() = default;
        std::unordered_set<gl::enum_type> enabled_toggles;
    };
}

