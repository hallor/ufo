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

    void DefaultAll();

protected:
    AppSettings();
    AppSettings & operator = (const AppSettings &copy) {};
    ~AppSettings();
};
