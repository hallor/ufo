#pragma once
#include <oal\al.h>
#include "ResourceBase.h"


class vSoundBufferResource : public vResource<ALuint>
{
public:
    vSoundBufferResource(const ALuint &res);
    ~vSoundBufferResource();

    /*
    Returns true when resource can be used safely, false otherwise
    */
    virtual bool IsValid() const;
protected:
};

class cSoundBuffer
{
public:
    cSoundBuffer(vSoundBufferResource *res);

    ALuint Get() const;
    vSoundBufferResource* GetRawResource() const { return m_Resource; };

    void Release() { delete this; }
protected:
    vSoundBufferResource* m_Resource;
};