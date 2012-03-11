#pragma once

#include "Property.h"

class EngineSettings
{
public:
    static EngineSettings &Get();
    static void Free();

    STATIC_PROPERTY(unsigned int, MaxSoundBuffers, 256);
    STATIC_PROPERTY(unsigned int, MaxSoundSources, 32);
    STATIC_PROPERTY(unsigned int, MaxQueuedSoundStreamBuffers, 3);
    STATIC_PROPERTY(float, LogicUpdateFrequency, 60);
    STATIC_PROPERTY(unsigned int, MaxLogicUpdatesPerFrame, 3);

    void DefaultAll();

protected:
    EngineSettings();
    EngineSettings & operator = (const EngineSettings &copy) { return *this; };
    ~EngineSettings();
};
