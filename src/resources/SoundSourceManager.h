#pragma once
#include "ManagerBase.h"
#include "SoundSource.h"
#ifdef _WIN32
#include <oal/al.h>
#else
#include <AL/al.h>
#endif

class cSoundSourceManager : public iManagerBase<ALuint>
{
    public:
    ~cSoundSourceManager() {};

    cSoundSource *Get();

    virtual void Update(float dt);
    virtual unsigned int GetResourcesCount() const;

    virtual bool ReloadAll();
    virtual bool ReloadResource(const std::string &id);

    virtual bool IsLoaded(const std::string &id) const;

    virtual void NotifyResourceParentChanged(vResource<ALuint> *res) const;

protected:

    virtual int FindResource(const std::string& id) const;
    virtual void RemoveResource(const std::string &id);
};
