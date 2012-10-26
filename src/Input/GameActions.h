#pragma once

namespace EGameAction
{
    enum TYPE
    {
    QUIT_GAME,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    LEVEL_NEXT,
    LEVEL_PREV,
    GAME_SPEED_PAUSED,
    GAME_SPEED_5S,
    GAME_SPEED_1MIN,
    GAME_SPEED_15MIN,
    GAME_SPEED_1HOUR,
    GAME_SPEED_1DAY,
    _COUNT,
    NONE 
    };
};