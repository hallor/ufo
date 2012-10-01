#include "game_pch.h"
#include "RendererEnums.h"

bool ERenderableType::IsValidType(ERenderableType::TYPE t)
{
    return t != ERenderableType::Unknown && t != ERenderableType::_COUNT;
};