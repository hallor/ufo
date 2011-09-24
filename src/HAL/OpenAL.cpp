#include "OpenAL.h"
#include "logger.h"
#ifdef _WIN32
#include <Windows.h>
#endif

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
    alGetError();

    alGenBuffers(1, &temp);
    
    if(alGetError() == AL_NO_ERROR)
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
    alGetError();

    alGenSources(1, &temp);

    if(alGetError() == AL_NO_ERROR)
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



