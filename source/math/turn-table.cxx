#include "../lib/glm/vec2.hxx"
#include "../lib/glm/mat4.hxx"
#include "turn-table.hxx"

namespace rt::math
{
    glm::mat4 rotation(turn_table const& tt)
    {
        auto x = tt.angles.x;
        auto y = tt.angles.y;
        auto z = tt.zoffset;

        auto cx = cos(x);
        auto cy = cos(y);
        auto sx = sin(x);
        auto sy = sin(y);
        auto ss = sx * sy;
        auto cc = cx * cy;
        auto sc = sx * cy;
        auto cs = cx * sy;

        return {
            {   cx  ,  ss  , -sc    , 0.0f },
            { 0.0f  ,  cy  ,  sy    , 0.0f },
            {   sx  , -cs  ,  cc    , 0.0f },
            {   sx*z, -cs*z,  cc*z-z, 1.0f },
        };
    }
}

