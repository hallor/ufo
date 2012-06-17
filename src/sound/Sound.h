#pragma once
#include <string>
#include "RenderableBase.h"
#include "SuperClass.h"
#include "OpenAL.h"
#include "Property.h"

class cSound : public vRenderable
{
    DECLARE_SUPER(vRenderable);

public:
    cSound();
    ~cSound();

    PROPERTY(float, Volume, 1.0f);
    PROPERTY(bool, Enabled, true);
    PROPERTY(bool, Looping, false);
    PROPERTY(ALsizei, Frequency, 22050); 

protected:

    std::string m_UID;
    std::string m_FileName;
    
    ALenum m_Format;
    float m_Length;
};