#pragma once
#include <vector>
#include <string>
#include "Property.h"
#include "IDGenerator.h"

class vRenderable;
class cSoundStream;

class cMusicPlayer 
{
public:
    cMusicPlayer();
    ~cMusicPlayer();

    virtual bool Initialize();

    virtual bool PlayMusic(const std::string &path);
    virtual void StopMusic();

    std::vector<vRenderable*> &GetStreams() { return m_Streams; }

    virtual void Update(float dt);

    virtual bool IsValid() const;

    virtual void Release();

    PROPERTY(float, MusicVolume, 1.0f);

protected:
    
    void UpdateStreams(float dt);
    void UpdateStream(cSoundStream *stream, float dt);

    int FindStream(const std::string &name) const;
    cSoundStream *GetStream(int index) const;
    
    int CreateStream();
    void DeleteStream(int index);

    void DeleteStreams();

    bool m_Initialized;

    cIdGenerator m_IdGenerator;

    cSoundStream *m_CurrentMusic;

    std::vector<vRenderable*> m_Streams;
};
