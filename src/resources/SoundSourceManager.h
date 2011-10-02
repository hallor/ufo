#pragma once
#include "SoundResourceManagerBase.h"
#include "IDGenerator.h"

class vSoundSourceResource;
class cSoundSource;

class cSoundSourceManager : public cSoundResourceManagerBase
{
public:
    cSoundSourceManager();

    /* 
    Creates new resource and returns pointer to new cSoundSource associated with it
    Releasing cSoundSource doesn't free it's associated resource, use ReleaseResource to do that
    It's callers responsibility to call Release() on cSoundSource that was acquired using this method, failing to do so will cause memory leaks
    cSoundSources will NOT be invalidated when their resource is internally deleted, therefore no calls to any methods touching inner resource should be made afterwards
    */
    cSoundSource *Get();

    /*
    Destroys and removes associated resource
    $res and every other cSoundSource associated with this resource is now considered invalid and unsafe to use
    */
    virtual void ReleaseResource(cSoundSource* res);

    /*
    Checks whether given sound source is valid
    Returns false when specified resource doesn't exist in manager's scope
    */
    virtual bool IsValidResource(cSoundSource *res) const;

protected:

    // Reloads resource
    virtual bool ReloadResource(unsigned int storage_index);
    
    /* 
    Returns pointer to new resource thats linked with oal source
    Returned resource is not claimed yet
    */
    vSoundSourceResource *CreateResource();

    virtual bool IsResourceTypeValid(vResource<ALuint> *res);

    /*
    Deletes associated oal source
    vSoundBufferResource is now in Deleted state
    */
    void UnloadResource(vResource<ALuint> * res);

    // ID generator, all resources must have unique names within resource manager instance
    cIdGenerator m_IdGenerator;
};
