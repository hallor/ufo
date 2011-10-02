#pragma once
#ifdef _WIN32
#include <oal/al.h>
#else
#include <AL/al.h>
#endif
#include <vector>
#include "ManagerBase.h"
#include "ResourceBase.h"

class cSoundResourceManagerBase : public iManagerBase<ALuint>
{
public:
    cSoundResourceManagerBase();
    virtual ~cSoundResourceManagerBase();
    
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
    Calling this method is safe, it will not cause associated resource proxies invalidation even if it fails
    */
    virtual bool ReloadResource(const std::string &id);
    
    /*
    Checks whether given resource is valid
    Returns false when specified resource doesn't exist in manager's scope
    */
    virtual bool IsValidResource(vResource<ALuint> *res) const;
    virtual bool IsValidResource(const std::string &id) const;
    
    /*
    Will be called when other manager takes ownership of resource, should not happen with sound sources or buffers
    Note: There might be id duplication as a result ownership change, one viable fix would be to either introduce static name generator
        for all managers, or to generate new name for claimed resource
    */
    virtual void NotifyResourceParentChanged(vResource<ALuint> *res);
    
    //Release all resources
    virtual void Release();

protected:
    
    // returns storage index of specified resource, -1 if it doesn't exist
    virtual int FindResource(const std::string& id) const;
    virtual int FindResource(vResource<ALuint> *res) const;
        
    virtual bool ReloadResource(unsigned int storage_index) = 0;
    /* 
    Claims ownership over given resource
    Returns false when resource is alredy claimed
    */
    virtual bool AddResource(vResource<ALuint> *res);

    // Returns true when resource type is valid for given manager
    virtual bool IsResourceTypeValid(vResource<ALuint> *res) = 0;
    /*
    Deletes associated sound resource
    Resource is now in Deleted state
    */
    virtual void UnloadResource(vResource<ALuint>* res) = 0;
    
    // Removes resource
    virtual void RemoveResource(unsigned int storage_index);
    
    std::vector<vResource<ALuint>*> m_Resources;
};
