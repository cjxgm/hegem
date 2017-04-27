#pragma once
#include "../lib/gl/gl.hh"
#include <unordered_set>

namespace rt::glu
{
    struct states_manager
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

