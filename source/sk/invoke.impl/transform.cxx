#include "../../lib/glm/op/transform.hxx"
#include "../../lib/std/any.hxx"
#include "../../math/direction.hxx"
#include "../../util/span.hxx"
#include "../../swing/op/transform.hxx"
#include "../op.hxx"
#include "model.hxx"
#include "util.hxx"
#include <utility>      // for std::move

namespace hegem::sk::op::invoke_impl
{
    namespace
    {
        auto median_or_custom(model const& m, float3 custom, bool use_median) -> glm::vec3
        {
            if (use_median) {
                if (m.vert_selection.empty()) return {};
                auto min = (*m.vert_selection.begin())->pos;
                auto max = min;
                for (auto vert: m.vert_selection) {
                    min = glm::min(min, vert->pos);
                    max = glm::max(max, vert->pos);
                }
                return (min + max) * 0.5f;
            } else {
                return to_glm(custom);
            }
        }
    }

    auto invoke(op_fields_transform_move const& fields, util::span<lib::any> args) -> lib::any
    {
        auto m = extract_or_croak<model>(args[0], "Argument must be a model.");

        auto offset = median_or_custom(m, fields.offset, fields.use_median);
        if (fields.reverse) offset = -offset;
        auto t = glm::translate(offset);

        for (auto vert: m.vert_selection)
            swing::affine_transform(vert, t);

        return std::move(m);
    }

    auto invoke(op_fields_transform_scale const& fields, util::span<lib::any> args) -> lib::any
    {
        auto m = extract_or_croak<model>(args[0], "Argument must be a model.");

        auto pivot = median_or_custom(m, fields.pivot, fields.use_median);
        auto amount = to_glm(fields.amount) * fields.uniform_amount;
        auto t = glm::translate(pivot)
            * glm::scale(amount)
            * glm::translate(-pivot);

        for (auto vert: m.vert_selection)
            swing::affine_transform(vert, t);

        return std::move(m);
    }

    auto invoke(op_fields_transform_rotate const& fields, util::span<lib::any> args) -> lib::any
    {
        auto m = extract_or_croak<model>(args[0], "Argument must be a model.");

        auto pivot = median_or_custom(m, fields.pivot, fields.use_median);
        auto axis = math::direction_type{to_glm(fields.axis)};
        auto amount = glm::radians(fields.amount);
        auto t = glm::translate(pivot)
            * glm::rotate(amount, *axis)
            * glm::translate(-pivot);

        for (auto vert: m.vert_selection)
            swing::affine_transform(vert, t);

        return std::move(m);
    }
}

