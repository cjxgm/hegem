#include "spawn.hh"

namespace rt::util
{
    void spawner::spawn(std::function<void()> f)
    {
        pool.push(std::move(f));
    }
}

