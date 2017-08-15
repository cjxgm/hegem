#pragma once
#include "../lib/glm/vec3.hh"
#include "../lib/glm/op/trig.hh"
#include "../lib/glm/op/geom.hh"
#include "direction.hh"
#include <random>

namespace rt::math
{
    struct uniform_sampler
    {
        uniform_sampler(float from, float to)
            : gen{(std::random_device{})()}
            , dist{from, to}
        {}

        float sample() { return dist(gen); }
        float operator () () { return sample(); }

    private:
        std::minstd_rand gen;
        std::uniform_real_distribution<float> dist;
    };

    struct normal_sampler
    {
        normal_sampler(float mean, float stddev)
            : gen{(std::random_device{})()}
            , dist{mean, stddev}
        {}

        float sample() { return dist(gen); }
        float operator () () { return sample(); }

    private:
        std::minstd_rand gen;
        std::normal_distribution<float> dist;
    };

    template <class Sampler>
    inline direction_type sample_cone(Sampler & samp, direction_type dir, float angle)
    {
        auto axis = (dir->z > 1.0f-1e-5f ? glm::vec3{1.0f, 0.0f, 0.0f} : glm::vec3{0.0, 0.0f, 1.0f});
        direction_type x = cross(*dir, axis);
        direction_type y = cross(*dir, *x);

        auto r = glm::tan(angle / 2.0f);
        auto s0 = samp() * r;
        auto s1 = samp() * r;
        direction_type sample = *dir + *x * s0 + *y * s1;
        return sample;
    }
}

