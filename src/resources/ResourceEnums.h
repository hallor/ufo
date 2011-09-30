#pragma once

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
        TextureAtlas
    };
}
