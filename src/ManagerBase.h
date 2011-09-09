#pragma once
#include <string>

template <class T>
class vResource;

template <class T>
class iManagerBase
{
public:
    virtual ~iManagerBase() {};

    virtual void Update(float dt) = 0;
    virtual unsigned int GetResourcesCount() const = 0;

    virtual bool ReloadAll() = 0;
    virtual bool ReloadResource(const std::string &id) = 0;

    virtual bool IsLoaded(const std::string &id) const = 0;

    virtual void NotifyResourceParentChanged(vResource<T> *res) const = 0;

protected:

    virtual unsigned int FindResource(const std::string& id) const = 0;
    virtual bool AddResource(vResource<T> *res) = 0;
    virtual void RemoveResource(const std::string &id) = 0;
};
