#pragma once
#include "../../lib/glm/vec3.hh"
#include "../../lib/std/any.hh"
#include "../op.hh"
#include <stdexcept>

namespace rt::sk
{
    namespace op::invoke_impl
    {
        template <class T>
        T extract_or_croak(lib::any& x, char const* error)
        {
            if (x.type() != typeid(T))
                throw std::runtime_error{error};
            return std::any_cast<T>(std::move(x));
        }

        inline auto to_glm(float3 a)
        {
            return glm::vec3{a[0], a[1], a[2]};
        }
    }
}

