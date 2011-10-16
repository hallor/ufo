#pragma once

#include <list>
#include "OpenAL.h"
#include "RendererBase.h"
#include "RenderableBase.h"
#include "Property.h"
#include "FixedArray.h"

class cSoundBuffer;
class cSoundSource;
class vSoundStreamProperties;
class cRawFile;

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
public:
    cSoundStream(const std::string &id);
    ~cSoundStream();

    virtual void BindFile(const std::string &path);

    virtual void PrepareForRendering();

    virtual void Update();

    // Sets all properties to initial value
    virtual void DefaultAllProperties();

    /* Synchronizes data with vSoundStreamProperties
       passing NULL is legal NOP */
    virtual void Synchronize(const vRenderingPropertiesBase *props);

    std::string GetId() const { return m_Id; };

    virtual void Release();

    // Properties synchronized with vSoundStreamProperties
    PROPERTY(ESoundState::TYPE, State, ESoundState::Stopped);

    // Properties not synchronized with vSoundStreamProperties
    PROPERTY(float, Volume, 1.0f);
    PROPERTY(bool, Enabled, true);
    PROPERTY(bool, Looping, false);
    PROPERTY(ESoundState::TYPE, WantedState, ESoundState::Stopped);

protected:
    
    virtual bool OpenStream(const std::string &path);
    virtual void CloseStream();

    virtual bool CreateRenderingProperties();

    vSoundStreamProperties *GetProperties();

    virtual cFixedArray<char>* GetFreeChunk();

    std::string m_Id;

    cRawFile m_BoundFile;
    
    std::list<cFixedArray<char>*> m_BoundChunks;
    std::list<cFixedArray<char>*> m_FreeChunks;
};

class vSoundStreamProperties : public vRenderingPropertiesBase
{
public:
    vSoundStreamProperties();
    virtual ~vSoundStreamProperties();
    
    /* Synchronizes properties with $stream for rendering
       passing NULL is legal NOP     */
    virtual void Synchronize(const vRenderable *stream);

    // Pops chunk from free chunks queue
    cFixedArray<char>* PopFreeChunk();
    // Pushes chunk to free chunks queue
    void AddFreeChunk(cFixedArray<char>* chunk);

    /* Pushes chunk with stream data to the end of the queue
       passing null as a parameter is legal NOP */
    void PushQueue(cFixedArray<char>* chunk);

    /* Peeks the next chunk of data in queue
       returns pointer to chunk or NULL when queue is empty  */
    cFixedArray<char>* PeekQueue() const;
    
    /* Pops next chunk from data queue
       return pointer to chunk or NULL when queue is empty   */
    cFixedArray<char>* PopQueue();
    /* Moves chunks from data queue to free elements queue   */
    void ClearQueue();

    // Returns count of queued data chunks
    unsigned int GetQueuedChunksCount() const { return m_ChunksInQueue.size(); }
    // Returns count of chunks in free elements queue
    unsigned int GetFreeChunksCount() const { return m_FreeChunks.size(); }

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
    std::list<cFixedArray<char>*> m_FreeChunks;

};