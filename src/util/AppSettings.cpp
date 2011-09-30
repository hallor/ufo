#include "AppSettings.h"

#ifdef _WIN32
#include <Windows.h>
#endif
#include <cstring>
#include "logger.h"

AppSettings *m_Instance = NULL;

AppSettings &AppSettings::Get()
{
    if(!m_Instance)
    {
        m_Instance = new AppSettings();
        m_Instance->DefaultAll();
    }

    return *m_Instance;
}

void AppSettings::Free()
{
    if(m_Instance)
        delete m_Instance;

    m_Instance = NULL;
}

void AppSettings::DefaultAll()
{
    LogInfo("Defaulting all settings");
    DefaultWindowWidth();
    DefaultWindowHeight();
    DefaultScrollSpeed();
		DefaultFPSLimit();
}

AppSettings::AppSettings()
{
    LogInfo("AppSettings created");
}

AppSettings::~AppSettings()
{
    LogInfo("AppSettings destroyed");
}
