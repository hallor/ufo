#include "ResourceEnums.h"

std::string EResourceType::ToString(EResourceType::TYPE t)
{
#ifndef WRITE
#define WRITE(p) case p: return #p;
    switch(t)
    {
        WRITE(OglTexture)
        WRITE(OalSoundBuffer)
        WRITE(OalSoundSource)
        WRITE(SDLSurface)
    }

#undef WRITE
#endif

    return "None";
}

EResourceType::TYPE EResourceType::FromString(const std::string &str)
{
#ifndef IF_RET
#define IF_RET(p) if(#p == str) return p; else
    IF_RET(OglTexture)
    IF_RET(OalSoundBuffer)
    IF_RET(OalSoundSource)
    IF_RET(SDLSurface)

#undef IF_RET
#endif

    return EResourceType::None;
}