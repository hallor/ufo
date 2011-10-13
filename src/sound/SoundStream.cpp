#include "SoundStream.h"

cSoundStream::cSoundStream()
{
    __super::m_RenderableType = ERenderableType::Sound;
    DefaultAllProperties();
};

cSoundStream::~cSoundStream()
{
};

bool cSoundStream::Initialize()
{
    return __super::Initialize();
};

void cSoundStream::PrepareForRendering()
{
    if(!IsValid())
        return;

    vSoundStreamProperties *props = GetProperties();
    if(!props)
        return;

    props->m_Volume = GetVolume();
    props->m_Enabled = GetEnabled();
}

void cSoundStream::DefaultAllProperties()
{
    DefaultVolume();
    DefaultEnabled();
    DefaultName();
};

bool cSoundStream::CreateRenderingProperties()
{
    if(GetProperties())
        return true;

    vSoundStreamProperties *tmp = new (std::nothrow) vSoundStreamProperties;
    
    __super::m_RenderingProperties = tmp;

    return GetProperties() != NULL;
};

vSoundStreamProperties *cSoundStream::GetProperties()
{
    return dynamic_cast<vSoundStreamProperties*>(__super::m_RenderingProperties);
};