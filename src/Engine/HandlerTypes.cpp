#include "game_pch.h"
#include "HandlerTypes.h"

bool EEngineHandler::IsValid(EEngineHandler::TYPE t)
{
    return t >= EEngineHandler::_First && t <= EEngineHandler::_Last;
};