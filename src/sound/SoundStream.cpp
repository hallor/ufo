#include "SoundStream.h"
#include "EngineSettings.h"

cSoundStream::cSoundStream(const std::string &id)
{
    __super::m_RenderableType = ERenderableType::Sound;
    m_Id = id;
    m_Format = AL_FORMAT_MONO8;
    DefaultAllProperties();
};

cSoundStream::~cSoundStream()
{
    Release();
};

void cSoundStream::BindFile(const std::string &path, ALenum format)
{
    if(!IsValid())
        return;

    m_Format = format;

    DeleteChunks();

    if(OpenStream(path))
        CreateChunks();
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
        ClearBoundQueue();
        return;
    }   

    FillChunks();
}

void cSoundStream::DefaultAllProperties()
{
    DefaultState();
    DefaultVolume();
    DefaultEnabled();
    DefaultLooping();
    DefaultFrequency();
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

    ClearBoundQueue();
    for(TArrayList::const_iterator it = stream_props->GetQueue().begin(); it != stream_props->GetQueue().end(); ++it)
    {
        m_FreeChunks.remove(*it);
        m_BoundChunks.push_back(*it);
    }
};

void cSoundStream::Release()
{
    CloseStream();
    DeleteChunks();
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

void cSoundStream::CreateChunks()
{
    DeleteChunks();
    
    if(CalculateChunkSize() == 0)
        return;

    for(unsigned int i = m_Chunks.size(); i < EngineSettings::GetMaxSoundStreamChunks(); ++i)
    {
        m_Chunks.push_back(new cFixedArray<char>(CalculateChunkSize()));
        m_FreeChunks.push_back(m_Chunks.back());
    }
}

void cSoundStream::DeleteChunks()
{
    m_BoundChunks.clear();
    m_FreeChunks.clear();

    for(TArrayList::iterator it = m_Chunks.begin(); it != m_Chunks.end(); ++it)
    {
        if(*it)
            delete (*it);
    }

    m_Chunks.clear();
}

void cSoundStream::FillChunks()
{
    while(!m_FreeChunks.empty())
    {
        cFixedArray<char>* chunk = m_FreeChunks.back();
        
        m_BoundFile.Read(chunk, GetLooping());
        m_BoundChunks.push_back(chunk);
        m_FreeChunks.pop_back();
    }
};

void cSoundStream::ClearBoundQueue()
{
    m_BoundChunks.clear();
    m_FreeChunks.clear();
    for(TArrayList::iterator it = m_Chunks.begin(); it != m_Chunks.end(); ++it)
    {
        m_FreeChunks.push_back(*it);
    }
};

unsigned int cSoundStream::CalculateChunkSize()
{
    switch(GetFormat())
    {
    case AL_FORMAT_MONO8: return GetFrequency() * 2;
    case AL_FORMAT_STEREO8: return GetFrequency() * 2 * 2;
    case AL_FORMAT_MONO16: return GetFrequency() * 2 * 2;
    case AL_FORMAT_STEREO16: return GetFrequency() * 4 * 2;
    }

    return 0;
}

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
    SetFrequency(stream->GetFrequency());
    SetFormat(stream->GetFormat());
};

void vSoundStreamProperties::PushQueue(cFixedArray<char> *chunk)
{
    if(m_ChunksInQueue.size() >= EngineSettings::GetDefaultMaxQueuedSoundStreamBuffers())
        return;

    m_ChunksInQueue.push_back(chunk);
};

cFixedArray<char> *vSoundStreamProperties::PeekQueue()
{
    if(m_ChunksInQueue.empty())
        return NULL;

    cFixedArray<char>* chunk = m_ChunksInQueue.front();
    return chunk;
};

cFixedArray<char> *vSoundStreamProperties::PopQueue()
{
    if(m_ChunksInQueue.empty())
        return NULL;

    cFixedArray<char>* chunk = m_ChunksInQueue.front();
    m_ChunksInQueue.pop_front();
    return chunk;
};

void vSoundStreamProperties::ClearQueue()
{
    m_ChunksInQueue.clear();
};

void vSoundStreamProperties::Clear()
{
    ClearQueue();

    DefaultVolume();
    DefaultEnabled();
    DefaultWantedState();
    DefaultState();
    DefaultFormat();
    DefaultFrequency();
    DefaultLooping();
};

