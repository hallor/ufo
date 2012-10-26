#include "game_pch.h"
#include "GameEnums.h"

std::string EGameSpeed::ToString(TYPE t)
{
    switch(t)
    {
    case SpeedPaused: return "Game paused";
    case Speed5s: return "Game speed 5 [s]";
    case Speed1min: return "Game speed 1 [min]";
    case Speed15min: return "Game speed 15 [min]";
    case Speed1hour: return "Game speed 1 [hour]";
    case Speed1day: return "Game speed 1 [day]";
    }

    return "";
}