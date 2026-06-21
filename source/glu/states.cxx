#include "states.hxx"
#include <utility>  // for std::move

namespace hegem::glu
{
    inline namespace
    {
        static constexpr gl::enum_type known_toggles[] = {
            gl::depth_test,
            gl::scissor_test,
            gl::blend,
            gl::dither,
            gl::enums::cull_face,
        };

        auto enable(gl::enum_type what, bool enabled) -> void
        {
            if (enabled) gl::enable(what);
            else gl::disable(what);
        }
    }

    states_manager& states_manager::instance()
    {
        static states_manager sm{};
        return sm;
    }

    auto states_manager::enable_only(std::unordered_set<gl::enum_type> enabled) -> void
    {
        for (auto t: known_toggles) {
            auto now = enabled.count(t);
            auto old = enabled_toggles.count(t);
            if (now ^ old) enable(t, now);
        }
        enabled_toggles = std::move(enabled);
    }
}

