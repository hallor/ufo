#include "SoundStream.h"
#include "RawFile.h"
#include "EngineSettings.h"

cSoundStream::cSoundStream(const std::string &id)
{
    __super::m_RenderableType = ERenderableType::Sound;
    m_Id = id;
    DefaultAllProperties();
};

cSoundStream::~cSoundStream()
{
};

void cSoundStream::BindFile(const std::string &path)
{
    if(!IsValid())
        return;

    OpenStream(path);
};

void cSoundStream::PrepareForRendering()
{
    if(!IsValid())
        return;

    vSoundStreamProperties *props = GetProperties();
    if(!props)
        return;

    props->Synchronize(this);
}

void cSoundStream::Update()
{
    if(!IsValid())
        return;

    if(!GetEnabled())
        return;

    if(GetWantedState() != ESoundState::Playing)
    {
        GetProperties()->ClearQueue();
        return;
    }

    cFixedArray<char> array(2 * 8 * 22050);
    
}

void cSoundStream::DefaultAllProperties()
{
    DefaultState();
    DefaultVolume();
    DefaultEnabled();
    DefaultLooping();
    DefaultWantedState(); 
};

void cSoundStream::Synchronize(const vRenderingPropertiesBase *props)
{
    if(!props)
        return;

    const vSoundStreamProperties *stream_props = dynamic_cast<const vSoundStreamProperties*>(props);

    if(!stream_props)
        return;

    SetState(stream_props->GetState());
};

void cSoundStream::Release()
{
    CloseStream();
};

bool cSoundStream::OpenStream(const std::string &path)
{
    if(path.empty())
        return false;

    return m_BoundFile.Open(path);
};

void cSoundStream::CloseStream()
{
    m_BoundFile.Release();
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

vSoundStreamProperties::vSoundStreamProperties()
{
};

vSoundStreamProperties::~vSoundStreamProperties()
{
    Clear();
};

void vSoundStreamProperties::Synchronize(const vRenderable *object)
{
    if(!object)
        return;

    const cSoundStream *stream = dynamic_cast<const cSoundStream*>(object);
    if(!stream)
        return;

    SetVolume(stream->GetVolume());
    SetEnabled(stream->GetEnabled());
    SetLooping(stream->GetLooping());
    SetWantedState(stream->GetWantedState());
};

void vSoundStreamProperties::Clear()
{
    m_ChunksInQueue.clear();
    m_FreeChunks.clear();

    DefaultVolume();
    DefaultEnabled();
    DefaultWantedState();
    DefaultState();
    DefaultFormat();
    DefaultFrequency();
    DefaultLooping();
};

