// no #pragma once intentionally
#include <stdexcept>
#define RT_UNREACHABLE() throw std::logic_error{"unreachable"};

