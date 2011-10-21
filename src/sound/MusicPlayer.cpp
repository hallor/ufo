#include "MusicPlayer.h"
#include "SoundStream.h"
#include "AppSettings.h"

cMusicPlayer::cMusicPlayer()
: m_IdGenerator("MusicStream")
{
    m_CurrentMusic = NULL;
    m_Initialized = false;
};

cMusicPlayer::~cMusicPlayer()
{
    Release();
};

bool cMusicPlayer::Initialize()
{
    DefaultMusicVolume();
    m_Initialized = true;
    return m_Initialized;
};

bool cMusicPlayer::PlayMusic(const std::string &path)
{
    if(!IsValid())
        return false;

    cSoundStream *stream = GetStream(FindStream(path));
    if(stream)
    {
        if(stream == m_CurrentMusic)
        {
            stream->SetWantedState(ESourceState::Playing);
        }
        else
        {
            m_CurrentMusic = stream;

            for(unsigned int i = 0; i < m_Streams.size(); ++i)
            {
                if(m_Streams[i] != stream)
                    stream->SetWantedState(ESourceState::Stopped);
            }
        }
    }
    else
    {
        stream = GetStream(CreateStream());
        if(!stream)
            return false;

        stream->BindFile(path, AL_FORMAT_STEREO16);
        stream->SetFrequency(44100);
        stream->SetLooping(true);
        stream->SetWantedState(ESourceState::Playing);
        m_CurrentMusic = stream;
    }

    return true;
};

void cMusicPlayer::StopMusic()
{
    if(!IsValid())
        return;

    if(m_CurrentMusic)
    {
        m_CurrentMusic->SetWantedState(ESourceState::Stopped);
        m_CurrentMusic = NULL;
    }
};

void cMusicPlayer::Update(float dt)
{
    if(!IsValid())
        return;

    UpdateStreams(dt);
};

bool cMusicPlayer::IsValid() const
{
    return m_Initialized;
};

void cMusicPlayer::Release()
{
    DeleteStreams();
};

void cMusicPlayer::UpdateStreams(float dt)
{
    for(unsigned int i = 0; i < m_Streams.size(); ++i)
    {
        UpdateStream(GetStream(i), dt);
    }
};

void cMusicPlayer::UpdateStream(cSoundStream *stream, float dt)
{
    if(!stream)
        return;
    
    if(stream->GetEnabled())
    {
        if(stream == m_CurrentMusic)
        {
            float volume = GetMusicVolume();
            stream->SetVolume(volume * AppSettings::GetMusicVolume());
        }
        else
        {
            stream->SetVolume(0.0f);
        }
    }
    else
    {
        stream->SetVolume(0.0f);
        stream->SetWantedState(ESourceState::Stopped);
    }

    stream->Update();
    stream->PrepareForRendering();
};

int cMusicPlayer::FindStream(const std::string &name) const
{
    for(unsigned int i = 0; i < m_Streams.size(); ++i)
    {
        cSoundStream *stream = GetStream(i);
        
        if(!stream)
            continue;
    
        if(stream->GetId() == name)
            return i;
    }

    return -1;
};

cSoundStream *cMusicPlayer::GetStream(int index) const
{
    if(index < 0 || index >= (int)m_Streams.size())
        return NULL;

    return dynamic_cast<cSoundStream*>(m_Streams[index]);
}

int cMusicPlayer::CreateStream()
{
    cSoundStream *stream = new (std::nothrow) cSoundStream(m_IdGenerator.Next());
    if(!stream)
        return -1;

    stream->Initialize();

    m_Streams.push_back(stream);

    return m_Streams.size() - 1;
};

void cMusicPlayer::DeleteStream(int index)
{
    if(index < 0 || index >= m_Streams.size())
        return;

    vRenderable *stream = m_Streams[index];
    m_Streams[index] = m_Streams.back();
    m_Streams.pop_back();

    if(stream)
        delete stream;
};

void cMusicPlayer::DeleteStreams()
{
    for(unsigned int i = 0; i < m_Streams.size(); ++i)
    {
        if(m_Streams[i])
            delete m_Streams[i];
    }

    m_Streams.clear();
};