#pragma once

#include "Property.h"

class EngineSettings
{
public:
    static EngineSettings &Get();
    static void Free();

    STATIC_PROPERTY(int, MaxSoundBuffers, 256);
    STATIC_PROPERTY(int, MaxSoundSources, 32);
    STATIC_PROPERTY(int, MaxQueuedSoundStreamBuffers, 3);
   
    void DefaultAll();

protected:
    EngineSettings();
    EngineSettings & operator = (const EngineSettings &copy) { return *this; };
    ~EngineSettings();
};
