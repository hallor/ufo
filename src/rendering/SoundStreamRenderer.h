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
    sStreamBinding(vRenderable *data, cSoundSource *source);
    sStreamBinding();

    vRenderable *m_Stream;
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

    void ClearAll();

    void ClearDataBindings();
    void ClearBinding(sStreamBinding &bnd);

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