#pragma once
#include <vector>

class vRenderable;

typedef std::vector<vRenderable*> TRenderableList;

class cMusicPlayer 
{
public:
    cMusicPlayer();
    ~cMusicPlayer();

    virtual bool Initialize();

    virtual bool PlayMusic(const std::string &path);

    const TRenderableList &GetStreams() const { return m_Streams; }

    virtual void Update(float dt);

    virtual void Release();

protected:

    int CreateStream();

    void DeleteStreams();

    TRenderableList m_Streams;
};