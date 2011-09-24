#pragma once
#include "ManagerBase.h"
#include "IDGenerator.h"
#include <oal/al.h>
#include <vector>

class cIdGenerator;
class vSoundBufferResource;
class cSoundBuffer;

class cSoundBufferManager : public iManagerBase<ALuint>
{
    public:
    cSoundBufferManager();
    ~cSoundBufferManager();

    /* 
    Creates new resource and returns pointer to new cSoundBuffer associated with it
    Releasing cSoundBuffer doesn't free it's associated resource, use ReleaseResource to do that
    It's callers responsibility to call Release() on cSoundBuffer that was acquired using this method, failing to do so will cause memory leaks
    cSoundBuffers will NOT be invalidated when their resource is internally deleted, therefore no calls to any methods touching inner resource should be made afterwards
    */
    cSoundBuffer *Get();

    
    // There's no use for that method yet
    virtual void Update(float dt);
    
    // Returns total resources count, both valid and invalid
    virtual unsigned int GetResourcesCount() const;

    /*
    Destroys and re-creates all resources 
    Follows the same set of rules as ReloadResource()
    */
    virtual bool ReloadAll();
    /*
    Destroys and recreates resource identified by $id
    Calling this method is safe, it will not cause associated cSoundBuffers invalidation even if it fails
    */
    virtual bool ReloadResource(const std::string &id);
    /*
    Destroys and removes associated resource
    $res and every other cSoundBuffer associated with this resource is now considered invalid and unsafe to use
    */
    virtual void ReleaseResource(cSoundBuffer* res);

    /*
    Checks whether given resource is valid
    Returns false when specified resource doesn't exist in manager's scope
    */
    virtual bool IsValidResource(vResource<ALuint> *res) const;
    virtual bool IsValidResource(const std::string &id) const;
    virtual bool IsValidResource(cSoundBuffer *res) const;
    
    /*
    Will be called when other manager takes ownership of resource, should not happen with sound buffers
    Note: There might be id duplication as a result ownership change, one viable fix would be to either introduce static name generator
        for all sound buffer managers, or to generate new name for claimed resource
    */
    virtual void NotifyResourceParentChanged(vResource<ALuint> *res);
    
    
    //Release all resources
    virtual void Release();

protected:
    // returns storage index of specified resource, -1 if it doesn't exist
    virtual int FindResource(const std::string& id) const;
    virtual int FindResource(vSoundBufferResource *res) const;

    /* 
    Claims ownership over given resource
    Returns false when resource is alredy claimed
    */
    virtual bool AddResource(vSoundBufferResource *res);
    
    // Removes resource
    virtual void RemoveResource(unsigned int storage_index);
    
    // Reloads resource
    virtual bool ReloadResource(unsigned int storage_index);
    
    /* 
    Returns pointer to new resource thats linked with oal buffer
    Returned resource is not claimed yet
    */
    vSoundBufferResource *CreateResource();

    /*
    Deletes associated oal buffer
    vSoundBufferResource is now in Deleted state
    */
    void UnloadResource(vSoundBufferResource* res);

    /* 
    Storage, vector should be optimal given that there is no need for many buffers
        (most sfx cards limit maximum sources to 32, so there's plenty of room for buffer reuse)
    */
    std::vector<vSoundBufferResource*> m_Resources;

    // ID generator, all resources must have unique names within resource manager instance
    cIdGenerator m_IdGenerator;
};