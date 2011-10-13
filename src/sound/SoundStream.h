#pragma once

#include <vector>
#include "RendererBase.h"
#include "RenderableBase.h"
#include "Property.h"

class cSoundBuffer;
class cSoundSource;
class vSoundStreamProperties;

class cSoundStream : public vRenderable
{
public:
    cSoundStream();
    ~cSoundStream();

    virtual bool Initialize();

    virtual void PrepareForRendering();

    PROPERTY(float, Volume, 1.0f);
    PROPERTY(bool, Enabled, true);
    PROPERTY(std::string, Name, "");

    virtual void DefaultAllProperties();

protected:

    virtual bool CreateRenderingProperties();

    vSoundStreamProperties *GetProperties();
};

class vSoundStreamProperties : public vRenderingPropertiesBase
{
public:
    float m_Volume;

    std::vector<void *> m_ChunksToPlay;
    std::vector<void *> m_ChunksDonePlaying;

    bool m_Enabled;

protected:
};