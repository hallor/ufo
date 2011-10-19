#pragma once

namespace ERenderableType
{
    enum TYPE
    {
        Unknown = 0,
        Object,
        Sound,
        SoundStream,
        _COUNT
    };

    bool IsValidType(TYPE t);
};