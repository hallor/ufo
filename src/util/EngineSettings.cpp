#include "game_pch.h"
#include "EngineSettings.h"

#ifdef _WIN32
#include <Windows.h>
#endif
#include <cstring>
#include "logger.h"

static EngineSettings *m_Instance = NULL;

EngineSettings &EngineSettings::Get()
{
    if(!m_Instance)
    {
        m_Instance = new EngineSettings();
        m_Instance->DefaultAll();
    }

    return *m_Instance;
}

void EngineSettings::Free()
{
    if(m_Instance)
        delete m_Instance;

    m_Instance = NULL;
}

void EngineSettings::DefaultAll()
{
    LogInfo("Defaulting all engine settings");
    DefaultMaxSoundBuffers();
    DefaultMaxSoundSources();
    DefaultMaxQueuedSoundStreamBuffers();
    DefaultLogicUpdateFrequency();
    DefaultMaxLogicUpdatesPerFrame();
}

EngineSettings::EngineSettings()
{
    LogInfo("EngineSettings created");
}

EngineSettings::~EngineSettings()
{
    LogInfo("EngineSettings destroyed");
}
