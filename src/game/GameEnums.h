#pragma once
#include <string>

namespace EGameSpeed
{
    enum TYPE
    {
        SpeedPaused = 0,
        Speed5s = 5,
        Speed1min = Speed5s * 12,
        Speed15min = Speed1min * 15,
        Speed1hour = Speed15min * 4,
        Speed1day = Speed1hour * 24,
    };

    std::string ToString(TYPE t);
};
