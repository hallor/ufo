#include "MusicPlayer.h"
#include "SoundStream.h"

cMusicPlayer::cMusicPlayer()
{
};

cMusicPlayer::~cMusicPlayer()
{
    Release();
};

bool cMusicPlayer::Initialize()
{
    return true;
};

bool cMusicPlayer::PlayMusic(const std::string &path)
{
    return false;
};

void cMusicPlayer::Update(float dt)
{
};

void cMusicPlayer::Release()
{
};

int cMusicPlayer::CreateStream()
{
    return -1;
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