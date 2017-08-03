#pragma once
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
}

