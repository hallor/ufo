#pragma once
#include <vector>
#include <string>
<<<<<<< HEAD
#include "Property.h"
#include "IDGenerator.h"
=======
>>>>>>> 6a55e75632d88d9088590a9a469ab83da6e434c0

class vRenderable;
class cSoundStream;

typedef std::vector<vRenderable*> TRenderableList;

class cMusicPlayer 
{
public:
    cMusicPlayer();
    ~cMusicPlayer();

    virtual bool Initialize();

    virtual bool PlayMusic(const std::string &path);
    virtual void StopMusic();

    const TRenderableList &GetStreams() const { return m_Streams; }

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

    cIdGenerator m_IdGenerator;

    cSoundStream *m_CurrentMusic;

    TRenderableList m_Streams;
};
