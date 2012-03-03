#pragma once

namespace ERenderableType
{
    enum TYPE
    {
        Unknown = 0,
        Object,
        Sound,
        SoundStream,
        Sprite3D,
        _COUNT
    };

    bool IsValidType(TYPE t);
};