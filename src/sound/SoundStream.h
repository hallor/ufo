#pragma once

#include <list>
#include "OpenAL.h"
#include "RendererBase.h"
#include "RenderableBase.h"
#include "Property.h"
#include "FixedArray.h"
#include "RawFile.h"

class cSoundBuffer;
class cSoundSource;
class vSoundStreamProperties;

namespace ESoundState
{
    enum TYPE
    {
        Playing = 0,
        Paused,
        Stopped,
        _COUNT
    };
};

class cSoundStream : public vRenderable
{
#ifndef _WIN32
	typedef vRenderable __super;
#endif
public:
    cSoundStream(const std::string &id);
    ~cSoundStream();

    virtual void BindFile(const std::string &path, ALenum format);

    virtual void PrepareForRendering();

    virtual void Update();

    // Sets all properties to initial value
    virtual void DefaultAllProperties();

    /* Synchronizes data with vSoundStreamProperties
       passing NULL is legal NOP */
    virtual void Synchronize(const vRenderingPropertiesBase *props);

    std::string GetId() const { return m_Id; };

    virtual void Release();

    ALenum GetFormat() const { return m_Format; }

    // Properties synchronized with vSoundStreamProperties
    PROPERTY(ESoundState::TYPE, State, ESoundState::Stopped);

    // Properties not synchronized with vSoundStreamProperties
    PROPERTY(float, Volume, 1.0f);
    PROPERTY(bool, Enabled, true);
    PROPERTY(bool, Looping, false);
    PROPERTY(ALsizei, Frequency, 22050);
    PROPERTY(ESoundState::TYPE, WantedState, ESoundState::Stopped);

protected:
    
    virtual bool OpenStream(const std::string &path);
    virtual void CloseStream();

    virtual bool CreateRenderingProperties();

    virtual void CreateChunks();
    virtual void DeleteChunks();
    virtual void FillChunks();

    virtual void ClearBoundQueue();

    unsigned int CalculateChunkSize();

    vSoundStreamProperties *GetProperties();
    
    std::string m_Id;

    cRawFile m_BoundFile;
    ALenum m_Format;

    typedef std::list<cFixedArray<char>*> TArrayList;
    
    std::list<cFixedArray<char>*> m_BoundChunks;
    std::list<cFixedArray<char>*> m_FreeChunks;
    std::list<cFixedArray<char>*> m_Chunks;
};

class vSoundStreamProperties : public vRenderingPropertiesBase
{
public:
    vSoundStreamProperties();
    virtual ~vSoundStreamProperties();
    
    /* Synchronizes properties with $stream for rendering
       passing NULL is legal NOP     */
    virtual void Synchronize(const vRenderable *stream);

    const std::list<cFixedArray<char>*> &GetQueue() const { return m_ChunksInQueue; }

    /* Pushes chunk with stream data to the end of the queue
       passing null as a parameter is legal NOP */
    void PushQueue(cFixedArray<char>* chunk);

    /* Peeks the next chunk of data in queue
       returns pointer to chunk or NULL when queue is empty  */
    cFixedArray<char>* PeekQueue();
    
    /* Pops next chunk from data queue
       return pointer to chunk or NULL when queue is empty   */
    cFixedArray<char>* PopQueue();
    /* Moves chunks from data queue to free elements queue   */
    void ClearQueue();

    // Returns count of queued data chunks
    unsigned int GetQueuedChunksCount() const { return m_ChunksInQueue.size(); }

    // Sets object to initial state
    void Clear();

    // Properties synchronized with cSoundStream
    PROPERTY(float, Volume, 1.0f);
    PROPERTY(bool, Enabled, true);
    PROPERTY(bool, Looping, false);    
    PROPERTY(ESoundState::TYPE, WantedState, ESoundState::Stopped);

    // Properties not synchronized with cSoundStream
    PROPERTY(ESoundState::TYPE, State, ESoundState::Stopped);
    PROPERTY(ALenum, Format, AL_FORMAT_MONO8);
    PROPERTY(ALsizei, Frequency, 22050);
    
protected:

    std::list<cFixedArray<char>*> m_ChunksInQueue;
};
