#pragma once
// modulus(a, b) is the remainder of a/b.
// It is guranteed that 0 <= modulus(a, b) < b.
#include <cmath>

namespace rt::math
{
    inline auto modulus(int a, int b) -> int
    {
        a %= b;
        if (a < 0) a += b;
        return a;
    }

    inline auto modulus(float a, float b) -> float
    {
        a = fmod(a, b);
        if (a < 0) a += b;
        return a;
    }
}

