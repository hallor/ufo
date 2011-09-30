#pragma once
#ifdef _WIN32
#include <oal/al.h>
#else
#include <AL/al.h>
#endif
#include "ResourceBase.h"

class vSoundSourceResource : public vResource<ALuint>
{
public:
    vSoundSourceResource(const ALuint &res);
    ~vSoundSourceResource();
};

class cSoundSource
{
public:
    cSoundSource(vSoundSourceResource *res);

    ALuint Get() const;
    vSoundSourceResource* GetRawResource() const { return m_Resource; };

    void Release() { delete this; };

protected:
    vSoundSourceResource* m_Resource;
};
