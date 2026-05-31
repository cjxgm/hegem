#pragma once
#include "timeline.hh"

namespace rt::kul
{
    struct spark_system
    {
        timeline pos_x;
        timeline pos_y;
        timeline pos_z;
        timeline radius;
        timeline emitting_color_r;
        timeline emitting_color_g;
        timeline emitting_color_b;
        timeline opacity;
        int num_particles{};
    };
}

