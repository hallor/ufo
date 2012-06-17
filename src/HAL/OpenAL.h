#pragma once

#ifdef _WIN32
#include <oal/al.h>
#include <oal/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include "IDGenerator.h"
#include "FixedArray.h"

struct sSourceProperties
{
    sSourceProperties();
    float m_Volume;
    float m_Pitch;
    bool  m_Looping;
};

namespace ESourceState
{
    enum TYPE
    {
        Playing = 0,
        Paused,
        Stopped,
        _COUNT
    };
};

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

    bool IsBuffer(ALuint buffer);
    bool IsSource(ALuint source);

    bool FillBufferData(ALuint buffer, FixedArray<char> *data, ALenum format, ALsizei frequency);
    bool SetSourceProperties(ALuint source, const sSourceProperties &props);

    int GetProcessedBuffersCount(ALuint source);
    ALuint PopBufferQueue(ALuint source);
    bool PushBufferQueue(ALuint source, ALuint buffer);

    bool SourcePlay(ALuint source);
    bool SourceStop(ALuint source);
    bool SourcePause(ALuint source);

    ESourceState::TYPE GetSourceState(ALuint source);

    ALenum GetLastError();

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
