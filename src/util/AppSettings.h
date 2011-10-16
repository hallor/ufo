#pragma once

#include "Property.h"

class AppSettings
{
public:
    static AppSettings &Get();
    static void Free();

    STATIC_PROPERTY(unsigned int, WindowWidth, 1280);
    STATIC_PROPERTY(unsigned int, WindowHeight, 960);
    STATIC_PROPERTY(unsigned int, FPSLimit, 60);
    STATIC_PROPERTY(float, ScrollSpeed, 5.0f);
    STATIC_PROPERTY(float, MusicVolume, 1.0f);
    STATIC_PROPERTY(float, MasterVolume, 1.0f);
    STATIC_PROPERTY(float, SoundVolume, 1.0f);

    void DefaultAll();

protected:
    AppSettings();
    AppSettings & operator = (const AppSettings &copy) { return *this; };
    ~AppSettings();
};
