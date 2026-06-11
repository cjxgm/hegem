#include "../../lib/glm/op/transform.hxx"
#include "../../lib/std/any.hxx"
#include "../../math/direction.hxx"
#include "../../util/span.hxx"
#include "../../swing/op/transform.hxx"
#include "../op.hxx"
#include "model.hxx"
#include "util.hxx"
#include "select.hxx"
#include <utility>      // for std::move

namespace hegem::skein::op::invoke_impl
{
    auto invoke(op_fields_repetition_array const& fields, util::span<lib::any> args) -> lib::any
    {
        auto tmpl = extract_or_croak<model>(args[0], "Argument must be a model.");

        model md;
        auto& m = md.hmesh;

        auto count = fields.count < 1 ? 1.0f : float(fields.count);
        auto offset = to_glm(fields.offset);
        auto recenter_transform = fields.recenter
            ? glm::translate(-0.5f * (count - 1.0f) * offset)
            : glm::mat4{};

        for (float i=0.0f; i<count; i++) {
            auto instance = tmpl.hmesh;
            auto t = recenter_transform * glm::translate(offset * i);
            swing::affine_transform_all(instance.any_body, t);
            m.extend(instance);
        }

        select_all(md);

        return std::move(md);
    }

    auto invoke(op_fields_repetition_spin const& fields, util::span<lib::any> args) -> lib::any
    {
        auto tmpl = extract_or_croak<model>(args[0], "Argument must be a model.");

        model md;
        auto& m = md.hmesh;

        auto count = fields.count < 1 ? 1.0f : float(fields.count);
        auto upper_bound = count;
        if (!fields.exclusive_end)
            upper_bound = (count == 1.0f ? 1.0f : count - 1.0f);

        for (float i=0.0f; i<count; i++) {
            auto instance = tmpl.hmesh;
            auto angle = -glm::radians(i / upper_bound * 360.0f * fields.fraction + fields.start);
            auto t = glm::translate(fields.radius * glm::vec3{glm::cos(angle), 0.0f, glm::sin(angle)});
            if (fields.local_rotation) t = t * glm::rotate(-angle, glm::vec3{0.0f, 1.0f, 0.0f});
            swing::affine_transform_all(instance.any_body, t);
            m.extend(instance);
        }

        select_all(md);

        return std::move(md);
    }
}

