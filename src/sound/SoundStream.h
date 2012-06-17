#pragma once

#include <list>
#include "OpenAL.h"
#include "RendererBase.h"
#include "RenderableBase.h"
#include "Property.h"
#include "FixedArray.h"
#include "RawFile.h"
#include "SuperClass.h"

class cSoundBuffer;
class cSoundSource;
class vSoundStreamProperties;


class cSoundStream : public vRenderable
{
    DECLARE_SUPER(vRenderable);
    
    typedef std::list<FixedArray<char>*> TArrayList;

public:
    cSoundStream(const std::string &id);
    virtual ~cSoundStream();

    /* Binds file specified by $path for streaming purposes
       creates chunks for data transport    */
    virtual void BindFile(const std::string &path, ALenum format);

    /* Synchronizes rendering data for renderer  */
    virtual void PrepareForRendering();

    /* Fills stream data chunks */
    virtual void Update();

    // Sets all properties to initial value
    virtual void DefaultAllProperties();

    /* Synchronizes data with vSoundStreamProperties
       passing NULL is legal NOP */
    virtual void Synchronize(const vRenderingPropertiesBase *props);

    // Retrieves stream UID
    std::string GetId() const { return m_Id; };

    // Releases all internal data, object ends up in initial state
    virtual void Release();
    
    // Retrieves format used when bindig file with BindFile() call
    ALenum GetFormat() const { return m_Format; }

    const TArrayList &GetDataQueue() const { return m_BoundChunks; }

    ////////////////////////////////////////////////////////
    // Properties synchronized with vSoundStreamProperties//
    ////////////////////////////////////////////////////////
    // Current state of the stream, updated and synchronized by renderer
    PROPERTY(ESourceState::TYPE, State, ESourceState::Stopped);

    ////////////////////////////////////////////////////////////
    // Properties not synchronized with vSoundStreamProperties//
    ////////////////////////////////////////////////////////////
    // Linear volume of sound
    PROPERTY(float, Volume, 1.0f);
    // Disabled sounds wont be updated or played
    PROPERTY(bool, Enabled, true);
    // When looping is set to true stream will start from the beginning as soon as it ends
    PROPERTY(bool, Looping, false);
    // Frequency of the sound, used to calculate initial size of chunks
    PROPERTY(ALsizei, Frequency, 22050);
    // Wanted stream state, will be taken into account by renderer
    PROPERTY(ESourceState::TYPE, WantedState, ESourceState::Stopped);

protected:
    
    /* Opens file stream
       returns false when specified file doesn't exist */
    virtual bool OpenStream(const std::string &path);
    // Closes previously opened file stream
    virtual void CloseStream();

    // Properties factory
    virtual bool CreateRenderingProperties();

    // Creates chunks
    virtual void CreateChunks();
    // Deletes chunks
    virtual void DeleteChunks();
    // Fills all free chunks with stream data
    virtual void FillChunks();

    // Checks whether given $chunk belongs to us
    virtual bool IsValidChunk(FixedArray<char>* chunk) const;

    // Invalidates all chunks in bound queue
    virtual void ClearBoundQueue();

    // Calculates chunks size for given frequency and format
    unsigned int CalculateChunkSize();

    // Returns renderingproperties
    vSoundStreamProperties *GetProperties();
    
    std::string m_Id;

    ALenum m_Format;
    
    cRawFile m_BoundFile;
    
    std::list<FixedArray<char>*> m_BoundChunks;
    std::list<FixedArray<char>*> m_FreeChunks;
    std::list<FixedArray<char>*> m_Chunks;
};

class vSoundStreamProperties : public vRenderingPropertiesBase
{
public:
    vSoundStreamProperties();
    virtual ~vSoundStreamProperties();
    
    /* Synchronizes properties with $stream for rendering
       passing NULL is legal NOP     */
    virtual void Synchronize(const vRenderable *stream);

    const std::list<FixedArray<char>*> &GetQueue() const { return m_ChunksInQueue; }

    /* Pushes chunk with stream data to the end of the queue
       passing null as a parameter is legal NOP */
    void PushQueue(FixedArray<char>* chunk);

    /* Peeks the next chunk of data in queue
       returns pointer to chunk or NULL when queue is empty  */
    FixedArray<char>* PeekQueue();
    
    /* Pops next chunk from data queue
       return pointer to chunk or NULL when queue is empty   */
    FixedArray<char>* PopQueue();
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
    PROPERTY(ESourceState::TYPE, WantedState, ESourceState::Stopped);

    // Properties not synchronized with cSoundStream
    PROPERTY(ESourceState::TYPE, State, ESourceState::Stopped);
    PROPERTY(ALenum, Format, AL_FORMAT_MONO8);
    PROPERTY(ALsizei, Frequency, 22050);
    
protected:

    std::list<FixedArray<char>*> m_ChunksInQueue;
};
