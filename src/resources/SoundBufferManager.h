#pragma once
#include "ManagerBase.h"
#include "SoundBuffer.h"
#include <oal/al.h>

class cSoundBufferManager : public iManagerBase<ALuint>
{
    public:
    ~cSoundBufferManager() {};

    cSoundBuffer *Get();

    virtual void Update(float dt);
    virtual unsigned int GetResourcesCount() const;

    virtual bool ReloadAll();
    virtual bool ReloadResource(const std::string &id);

    virtual bool IsLoaded(const std::string &id) const;

    virtual void NotifyResourceParentChanged(vResource<ALuint> *res) const;

protected:

    virtual unsigned int FindResource(const std::string& id) const;
    virtual bool AddResource(vResource<ALuint> *res);
    virtual void RemoveResource(const std::string &id);
};