#pragma once
#include <oal\al.h>
#include "ResourceBase.h"


class vSoundBufferResource : public vResource<ALuint>
{
public:
    vSoundBufferResource();
    ~vSoundBufferResource();

protected:
};

class cSoundBuffer
{
public:
    cSoundBuffer(vSoundBufferResource *res);

    ALuint Get() const;
    vSoundBufferResource* GetRawResource() const { return m_Resource; };

protected:
    vSoundBufferResource* m_Resource;
};