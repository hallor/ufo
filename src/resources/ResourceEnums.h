#pragma once
#include <string>

namespace EResourceState
{
    enum TYPE
    {
        NotLoaded = 0,
        LoadingInProgress,
        Ok,
        Deleted,
        _COUNT
    };
}

namespace EResourceType
{
    enum TYPE
    {
        None = 0,
        OglTexture,
        OalSoundBuffer,
        OalSoundSource,
        SDLSurface,
        _COUNT
    };

    std::string ToString(TYPE t);
    TYPE FromString(const std::string &str);
}
