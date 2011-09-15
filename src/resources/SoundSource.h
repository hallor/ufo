#pragma once
#include <oal\al.h>
#include "ResourceBase.h"

class vSoundSourceResource : public vResource<ALuint>
{
public:
    vSoundSourceResource();
    ~vSoundSourceResource();
};

class cSoundSource
{
public:
    cSoundSource(vSoundSourceResource *res);

    ALuint Get() const;
    vSoundSourceResource* GetRawResource() const { return m_Resource; };

protected:
    vSoundSourceResource* m_Resource;
};