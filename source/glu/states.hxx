#pragma once
#include "../lib/gl/gl.hxx"
#include "../tool/constraints.hxx"
#include <unordered_set>

namespace hegem::glu
{
    struct states_manager: tool::non_transferable
    {
        static states_manager& instance();

        // Enable everything in `enabled`, disable anything else.
        // `enabled` must be a subset of `known_toggles`,
        // it's undefined behavior otherwise.
        auto enable_only(std::unordered_set<gl::enum_type> enabled) -> void;

    private:
        states_manager() = default;
        std::unordered_set<gl::enum_type> enabled_toggles;
    };
}

