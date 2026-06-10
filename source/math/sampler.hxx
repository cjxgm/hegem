#pragma once
#include "../lib/glm/vec3.hxx"
#include "../lib/glm/mat3.hxx"
#include "../lib/glm/op/trig.hxx"
#include "../lib/glm/op/geom.hxx"
#include "direction.hxx"
#include "constants.hxx"
#include "local-space.hxx"
#include <random>
#include <cmath>

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
        std::mt19937_64 gen;
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
        std::mt19937_64 gen;
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

    template <class Sampler>
    inline direction_type sample_hemisphere(Sampler& sample01, direction_type const& normal)
    {
        auto rnd1 = 2.0f * math::pi * sample01();
        auto rnd2 = sample01();
        auto s2 = std::sqrt(rnd2);
        auto local = glm::vec3{
            std::cos(rnd1) * s2,
            std::sin(rnd1) * s2,
            std::sqrt(1.0f - rnd2),
        };
        auto local_to_world = local_space(normal);
        return local_to_world * local;
    }

    template <class Sampler>
    inline direction_type sample_sphere(Sampler & samp)
    {
        while (true) {
            auto s0 = samp();
            auto s1 = samp();
            auto s2 = samp();

            if (s0 == 0.0f && s1 == 0.0f && s2 == 0.0f) continue;

            direction_type sample{glm::vec3{s0, s1, s2}};
            return sample;
        }
    }
}

