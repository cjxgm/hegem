#pragma once
// Subdivision surface
#include "../hemesh.fwd.hxx"

namespace hegem::swing
{
    inline namespace op
    {
        inline namespace subsurf
        {
            // The resulting hemesh is solely made up of triangles.
            auto triangulate(hemesh m) -> hemesh;

            // The resulting hemesh is solely made up of convex polygons.
            // Holes are removed and concave are cut into convex.
            //
            // In contrary to triangulation, The algorithm tries its best
            // to reduce the amount of polygons, that is, maximize
            // the number of vertices of each polygon.
            auto convexify(hemesh& m) -> void;

            // ASSUMPTIONS:
            //   m contains only simple polygons (no holes, no concaves)
            auto subdivide_catmull_clark(hemesh& m) -> void;
        }
    }
}

