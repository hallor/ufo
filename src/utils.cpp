#include "utils.h"
using namespace Utils;
/** Assumes 32-bit integer rounded down */
unsigned int Utils::fastlog2floor(unsigned int number)
{
    unsigned int r = 0;

    while (number >>= 1) // unroll for more speed...
    {
        r++;
    }

    return r;
}

/** Normalizes resolution of "raster" to nearest power-of-2 */
unsigned int Utils::normalizeResolution(unsigned int resolution)
{
    unsigned int r = fastlog2floor(resolution);
    unsigned int size = 1 << r;

    if (resolution > size)
        size <<= 1;
    return size;
}

