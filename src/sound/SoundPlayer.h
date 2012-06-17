#pragma once
#include <vector>
#include "Property.h"
#include "IDGenerator.h"

class cSoundPlayer
{
public:
    cSoundPlayer();
    ~cSoundPlayer();

    virtual void Update(float dt);

    virtual bool PlaySound(const std::string &id, float volume = 1.0f);

    PROPERTY(float, SoundsVolume, 1.0f);

protected:
    
    std::vector<std::string> m_SoundsToPlay;
};