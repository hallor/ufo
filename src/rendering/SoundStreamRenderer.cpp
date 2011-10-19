#include "SoundStreamRenderer.h"
#include "SoundBufferManager.h"
#include "SoundSourceManager.h"
#include "SoundSource.h"
#include "SoundBuffer.h"
#include "SoundStream.h"
#include "OpenAL.h"
#include "EngineSettings.h"

sStreamBinding::sStreamBinding(vRenderable *data, cSoundSource *source)
{
    m_Stream = data;
    m_Source = source;
    m_Marked = false;
};

sStreamBinding::sStreamBinding()
{
    m_Stream = NULL;
    m_Source = NULL;
    m_Marked = false;
};

cSoundStreamRenderer::cSoundStreamRenderer()
{
    ClearAll();
};

cSoundStreamRenderer::~cSoundStreamRenderer()
{
    Release();
};

bool cSoundStreamRenderer::Initialize(cSoundBufferManager *bufferman, cSoundSourceManager *srcman)
{
    if(!bufferman || !srcman)
        return false;

    if(IsValid() && bufferman == m_BufferManager && srcman == m_SourceManager)
        return true;

    Release();

    m_BufferManager = bufferman;
    m_SourceManager = srcman;

    return IsValid();
};

bool cSoundStreamRenderer::IsValid() const
{
    // TODO iManagerBase::IsValid()
    bool man_valid = m_BufferManager ? true : false;
    man_valid &= m_SourceManager ? true : false;
    
    return man_valid & OpenAL::Get().IsInitialized();
};

void cSoundStreamRenderer::OnFrame(float dt)
{
    if(!IsValid())
        ClearAll();
};

void cSoundStreamRenderer::Render(const vRenderable &object)
{
    if(!IsValid())
        return;
};

void cSoundStreamRenderer::Release()
{
    ReleaseSources();
    ReleaseBuffers();

    ClearAll();

    m_BufferManager = NULL;
    m_SourceManager = NULL;
};

void cSoundStreamRenderer::ClearAll()
{
    ClearDataBindings();
    m_Sources.Clear();
    m_Buffers.Clear();
};

void cSoundStreamRenderer::ClearDataBindings()
{
    for(unsigned int i = 0; i < m_DataBindings.size(); ++i)
        ClearBinding(m_DataBindings[i]);

    m_DataBindings.clear();
};

void cSoundStreamRenderer::ClearBinding(sStreamBinding &bnd)
{
    m_Sources.Return(bnd.m_Source);
    bnd.m_Source = NULL;

    bnd.m_Stream = NULL;

    bnd.m_Buffers.FreeAll();
    while(bnd.m_Buffers.HasFree())
    {
        cSoundBuffer *buf = NULL;
        bnd.m_Buffers.Get(buf);
        m_Buffers.Return(buf);
    }
    bnd.m_Buffers.Clear();

    bnd.m_Marked = false;
}

void cSoundStreamRenderer::ReleaseSources()
{
    m_Sources.FreeAll();
    while(m_Sources.HasFree())
    {
        cSoundSource *src = NULL;
        m_Sources.Get(src);

        if(m_SourceManager)
            m_SourceManager->ReleaseResource(src);
        
        if(src)
            src->Release();
    }

    m_Buffers.Clear();
};

void cSoundStreamRenderer::ReleaseBuffers()
{
    m_Buffers.FreeAll();
    while(m_Buffers.HasFree())
    {
        cSoundBuffer *buf = NULL;
        m_Buffers.Get(buf);

        if(m_BufferManager)
            m_BufferManager->ReleaseResource(buf);
        
        if(buf)
            buf->Release();
    }

    m_Buffers.Clear();
};


void cSoundStreamRenderer::TryCreateBuffers(unsigned int count)
{
    if(!IsValid())
        return;

    if(m_Buffers.HasFree())
        return;

    for(unsigned int i = 0; i < count; ++i)
    {
        cSoundBuffer *buf = m_BufferManager->Get();
        if(!buf)
            return;

        m_Buffers.Add(buf);
    }
};

void cSoundStreamRenderer::TryCreateSources(unsigned int count)
{
    if(!IsValid())
        return;

    if(m_Sources.HasFree())
        return;

    for(unsigned int i = 0; i < count; ++i)
    {
        cSoundSource *src = m_SourceManager->Get();
        if(!src)
            return;

        m_Sources.Add(src);
    }
};
