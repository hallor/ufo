#pragma once
#include <vector>
#include "RendererBase.h"
#include "ObjectPool.h"

class vRenderingPropertiesBase;
class cSoundSource;
class cSoundBuffer;

class cSoundStream;

class cSoundBufferManager;
class cSoundSourceManager;

struct sStreamBinding
{
    sStreamBinding(const vRenderable *data, cSoundSource *source);
    sStreamBinding();

    const vRenderable *m_Stream;
    cSoundSource *m_Source;
    ObjectPool<cSoundBuffer*> m_Buffers;
    bool m_Marked;
};

class cSoundStreamRenderer : public iRendererBase
{
public:
    cSoundStreamRenderer();
    virtual ~cSoundStreamRenderer();

    virtual bool Initialize(cSoundBufferManager *bufferman, cSoundSourceManager *srcman);

    virtual bool IsValid() const;

    virtual void OnFrame(float dt);

    virtual void Render(const vRenderable &object);

    virtual void Release();

protected:
    
    void UpdateStreams();

    int FindStreamBinding(const vRenderable &stream);
    
    void StopStream(sStreamBinding &binding);
    void PauseStream(sStreamBinding &stream);
    void PlayStream(sStreamBinding &stream);

    void ClearDataBindings();
    void RemoveUnmarkedBindings();
    void ClearBinding(sStreamBinding &bnd);
    void UnmarkAllBindings();

    int CreateBinding(const vRenderable &stream) { return -1; }

    void ReleaseSources();
    void ReleaseBuffers();

    void TryCreateSources(unsigned int count);
    void TryCreateBuffers(unsigned int count);

    cSoundBufferManager *m_BufferManager;
    cSoundSourceManager *m_SourceManager;

    std::vector<sStreamBinding> m_DataBindings;
    ObjectPool<cSoundSource*> m_Sources;
    ObjectPool<cSoundBuffer*> m_Buffers;
};