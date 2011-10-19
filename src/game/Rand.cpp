#include <cstdlib>

#include "Rand.h"

Rand::Rand(int seed)
{
    srand(seed);
}

int Rand::get() const
{
    return rand();
}
