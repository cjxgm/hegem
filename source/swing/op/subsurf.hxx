#pragma once
// Subdivision surface
#include "../hemesh.fwd.hxx"

namespace rt::swing
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
            void convexify(hemesh& m);

            // ASSUMPTIONS:
            //   m contains only simple polygons (no holes, no concaves)
            void subdivide_catmull_clark(hemesh& m);
        }
    }
}

