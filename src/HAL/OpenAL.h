#pragma once

#ifdef _WIN32
#include <oal/al.h>
#include <oal/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include "IDGenerator.h"

class OpenAL
{
public:
    static OpenAL &Get();
    static void Free();

    bool Initialize();

    ALuint CreateBuffer();
    ALuint CreateSource();

    void DeleteBuffer(ALuint buffer);
    void DeleteSource(ALuint source);

    bool IsInitialized() const;

    void Release();

protected:

    OpenAL();
    ~OpenAL();
    
    OpenAL(const OpenAL &cp) {};
    OpenAL &operator = (const OpenAL &cp) { return *this; }

    bool CreateDevice();
    bool CreateContext();

    void ReleaseDevice();
    void ReleaseContext();

    ALCdevice *m_Device;
    ALCcontext *m_Context;

    int m_ActiveBuffers;
    int m_ActiveSources;
};
