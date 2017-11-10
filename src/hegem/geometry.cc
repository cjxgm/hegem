#include "../lib/glm/op/geom.hh"
#include "geometry.hh"
#include "primitive.hh"
#include "list.hh"

namespace rt::hegem
{
    inline namespace geometry_operator
    {
        namespace
        {
            position_type yzx(position_type a) { return { a.y, a.z, a.x }; }
        }

        direction_type normal(hege_type* hege, float eps, direction_type fallback)
        {
            // Use Newell's method to calculate the normal.
            // It works for concave polygons and near-parallel edges.
            //
            // Tampieri, Filippo. 1992. "Newell's Method for Computing
            // the Plane Equation.". Graphics Gems III 231–232. Academic Press.
            offset_type normal;
            for (auto& h: list::iterate(hege)) {
                auto u = h.      start->pos;
                auto v = h.next->start->pos;
                auto u0 = yzx(u);
                auto v0 = yzx(v);
                auto u1 = yzx(u0);
                auto v1 = yzx(v0);
                normal += (u0 - v0) * (u1 + v1);
            }

            if (dot(normal, normal) <= eps*eps) return fallback;
            return normal;
        }

        bool is_same_side(direction_type dir0, direction_type dir1, float eps)
        {
            auto sideness = dot(*dir0, *dir1);
            return (sideness > eps);
        }

        bool is_same_side(
            direction_type normal, position_type origin,    // a plane
            position_type point,
            float eps)
        {
            auto offset = point - origin;
            if (dot(offset, offset) <= eps) return false;
            return is_same_side(normal, offset, eps);
        }
    }
}

