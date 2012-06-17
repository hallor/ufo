#include "OpenAL.h"
#include "logger.h"
#ifdef _WIN32
#include <Windows.h>
#endif

sSourceProperties::sSourceProperties()
{
    m_Volume = 1.0f;
    m_Pitch = 1.0f;
    m_Looping = false;
}

OpenAL *Instance = NULL;

OpenAL &OpenAL::Get()
{
    if(!Instance)
        Instance = new OpenAL();

    return *Instance;
}

void OpenAL::Free()
{
    if(Instance)
        delete Instance;

    Instance = NULL;
}

bool OpenAL::Initialize()
{
    if(IsInitialized())
    {
        LogError("OAL reinitialization detected, skipping");
        return true;
    }

    LogInfo("Initializing OAL...");

    CreateDevice();
    CreateContext();

    return IsInitialized();
}

ALuint OpenAL::CreateBuffer()
{
    if(!IsInitialized())
        return 0;
    
    ALuint temp;
    GetLastError();

    alGenBuffers(1, &temp);
    
    if(GetLastError() == AL_NO_ERROR)
    {
        ++m_ActiveBuffers;
        LogInfo("Created OAL buffer, %i active buffers", m_ActiveBuffers);
        return temp;
    }

    return 0;
}

ALuint OpenAL::CreateSource()
{
    if(!IsInitialized())
        return 0;
    
    ALuint temp;
    GetLastError();

    alGenSources(1, &temp);

    if(GetLastError() == AL_NO_ERROR)
    {
        ++m_ActiveSources;
        LogInfo("Created OAL source, %i active sources", m_ActiveSources);
        return temp;
    }

    return 0;
}

void OpenAL::DeleteBuffer(ALuint buffer)
{
    if(!IsInitialized())
        return;

    if(!alIsBuffer(buffer))
        return;

    alDeleteBuffers(1, &buffer);
    --m_ActiveBuffers;

    LogInfo("Deleted OAL buffer, %i active buffers left", m_ActiveBuffers);
}

void OpenAL::DeleteSource(ALuint source)
{
    if(!IsInitialized())
        return;

    if(!alIsSource(source))
        return;

    alDeleteSources(1, &source);
    --m_ActiveSources;

    LogInfo("Deleted OAL source, %i active sources left", m_ActiveSources);
}

bool OpenAL::IsBuffer(ALuint buffer)
{
    if(!IsInitialized())
        return false;

    return alIsBuffer(buffer) == AL_TRUE;
}

bool OpenAL::IsSource(ALuint source)
{
    if(!IsInitialized())
        return false;

    return alIsSource(source) == AL_TRUE;
}

bool OpenAL::FillBufferData(ALuint buffer, FixedArray<char> *data, ALenum format, ALsizei frequency)
{
    if(!IsInitialized())
        return false;

    if(IsBuffer(buffer) != AL_TRUE)
        return false;

    if(!data)
        return false;

    GetLastError();

    alBufferData(buffer, format, data->GetDataPointer(), data->GetSize(), frequency);

    return GetLastError() == AL_NO_ERROR;
};

bool OpenAL::SetSourceProperties(ALuint source, const sSourceProperties &props)
{
    if(!IsInitialized())
        return false;

    if(IsSource(source) != AL_TRUE)
        return false;

    GetLastError();

    alSourcef(source, AL_GAIN, props.m_Volume);
    alSourcef(source, AL_PITCH, props.m_Pitch);
    alSourcei(source, AL_LOOPING, props.m_Looping ? AL_TRUE : AL_FALSE);

    return GetLastError() == AL_NO_ERROR;
};

int OpenAL::GetProcessedBuffersCount(ALuint source)
{
    if(!IsInitialized())
        return 0;

    if(IsSource(source) != AL_TRUE)
        return 0;

    GetLastError();

    ALint processed = 0;

    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

    return GetLastError() == AL_NO_ERROR ? processed : 0;
};

ALuint OpenAL::PopBufferQueue(ALuint source)
{
    if(!GetProcessedBuffersCount(source))
        return 0;
    
    GetLastError();

    ALuint buffer = 0;

    alSourceUnqueueBuffers(source, 1, &buffer);

    return GetLastError() == AL_NO_ERROR ? buffer : 0;
};

bool OpenAL::PushBufferQueue(ALuint source, ALuint buffer)
{
    if(!IsInitialized())
        return false;

    if(!IsSource(source) || !IsBuffer(buffer))
        return false;

    GetLastError();
    
    alSourceQueueBuffers(source, 1, &buffer);

    return GetLastError() == AL_NO_ERROR;
};

bool OpenAL::SourcePlay(ALuint source)
{
    if(!IsInitialized())
        return false;

    if(!IsSource(source))
        return false;

    if(GetSourceState(source) == ESourceState::Playing)
        return true;

    GetLastError();
    alSourcePlay(source);

    return GetLastError() == AL_NO_ERROR && GetSourceState(source) == ESourceState::Playing;
};

bool OpenAL::SourceStop(ALuint source)
{
    if(!IsInitialized())
        return false;

    if(!IsSource(source))
        return false;

    if(GetSourceState(source) == ESourceState::Stopped)
        return true;

    GetLastError();
    alSourceStop(source);

    return GetLastError() == AL_NO_ERROR && GetSourceState(source) == ESourceState::Stopped;
};

bool OpenAL::SourcePause(ALuint source)
{
    if(!IsInitialized())
        return false;

    if(!IsSource(source))
        return false;

    if(GetSourceState(source) == ESourceState::Paused)
        return true;

    GetLastError();
    alSourcePause(source);

    return GetLastError() == AL_NO_ERROR && GetSourceState(source) == ESourceState::Paused;
};

ESourceState::TYPE OpenAL::GetSourceState(ALuint source)
{
    if(!IsInitialized())
        return ESourceState::_COUNT;

    if(!IsSource(source))
        return ESourceState::_COUNT;

    GetLastError();

    ALint state = 0;

    alGetSourcei(source, AL_SOURCE_STATE, &state);

    if(GetLastError() != AL_NO_ERROR)
        return ESourceState::_COUNT;

    switch(state)
    {
    case AL_PLAYING: return ESourceState::Playing;
    case AL_PAUSED: return ESourceState::Paused;
    case AL_STOPPED: return ESourceState::Stopped;
    default:    return ESourceState::_COUNT;
    }
}

ALenum OpenAL::GetLastError()
{
    return alGetError();
}

bool OpenAL::IsInitialized() const
{
    if(!m_Device || !m_Context)
        return false;

    return true;
}

void OpenAL::Release()
{
    LogInfo("Releasing OAL");
    
    if(m_ActiveBuffers != 0 || m_ActiveSources != 0)
    {
        LogError("Fatal OAL error: hanging references detected, %i buffers, %i sources", m_ActiveBuffers, m_ActiveSources);
    }

    ReleaseDevice();
}

OpenAL::OpenAL()
{
    m_Device = NULL;
    m_Context = NULL;

    m_ActiveBuffers = 0;
    m_ActiveSources = 0;
}

OpenAL::~OpenAL()
{
    Release();
}

bool OpenAL::CreateDevice()
{
    ReleaseDevice();

    m_Device = alcOpenDevice(NULL);

    if(m_Device)
        LogInfo("Opened OAL device");
 
    return m_Device != NULL;
}

bool OpenAL::CreateContext()
{
    ReleaseContext();

    if(!m_Device)
    {
        LogError("Couldn't create OAL context, device not opened");
        return false;
    }

    m_Context = alcCreateContext(m_Device, NULL);

    if(m_Context)
    {
        LogInfo("Created OAL context");
        alcMakeContextCurrent(m_Context);
    }

    return m_Context != NULL;
}

void OpenAL::ReleaseDevice()
{
    ReleaseContext();

    if(m_Device)
    {
        LogInfo("Releasing OAL device");
        alcCloseDevice(m_Device);
        m_Device = NULL;
    }
}

void OpenAL::ReleaseContext()
{
    if(m_Context)
    {
        LogInfo("Releasing OAL context");
        alcMakeContextCurrent(NULL);
        alcDestroyContext(m_Context);
        m_Context = NULL;
    }
}



