#pragma once
#include "SoundResourceManagerBase.h"
#include "IDGenerator.h"

class vSoundBufferResource;
class cSoundBuffer;

class cSoundBufferManager : public cSoundResourceManagerBase
{
public:
    cSoundBufferManager();

    /* 
    Creates new resource and returns pointer to new cSoundBuffer associated with it
    Releasing cSoundBuffer doesn't free it's associated resource, use ReleaseResource to do that
    It's callers responsibility to call Release() on cSoundBuffer that was acquired using this method, failing to do so will cause memory leaks
    cSoundBuffers will NOT be invalidated when their resource is internally deleted, therefore no calls to any methods touching inner resource should be made afterwards
    */
    cSoundBuffer *Get();

    /*
    Destroys and removes associated resource
    $res and every other cSoundBuffer associated with this resource is now considered invalid and unsafe to use
    */
    virtual void ReleaseResource(cSoundBuffer* res);

    /*
    Checks whether given sound buffer is valid
    Returns false when specified resource doesn't exist in manager's scope
    */
    virtual bool IsValidResource(cSoundBuffer *res) const;

protected:
    
    // Reloads resource
    virtual bool ReloadResource(unsigned int storage_index);
    
    /* 
    Returns pointer to new resource thats linked with oal buffer
    Returned resource is not claimed yet
    */
    vSoundBufferResource *CreateResource();

    virtual bool IsResourceTypeValid(vResource<ALuint> *res);
    /*
    Deletes associated oal buffer
    vSoundBufferResource is now in Deleted state
    */
    void UnloadResource(vResource<ALuint> * res);
    
    // ID generator, all resources must have unique names within resource manager instance
    cIdGenerator m_IdGenerator;
};
