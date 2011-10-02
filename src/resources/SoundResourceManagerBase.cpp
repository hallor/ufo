#include "SoundResourceManagerBase.h"

cSoundResourceManagerBase::cSoundResourceManagerBase()
{
}

cSoundResourceManagerBase::~cSoundResourceManagerBase()
{
    // Release all remainig resources
    Release(); 
}

void cSoundResourceManagerBase::Update(float dt)
{
    // No updates needed here
}

unsigned int cSoundResourceManagerBase::GetResourcesCount() const
{
    return m_Resources.size();
}

bool cSoundResourceManagerBase::ReloadAll()
{    
    bool succed = true;
    for(unsigned int i = 0; i < m_Resources.size(); ++i)
    {
        succed &= ReloadResource(i);
    }

    return succed;
}

bool cSoundResourceManagerBase::ReloadResource(const std::string &id)
{
    return ReloadResource(FindResource(id));
}

bool cSoundResourceManagerBase::IsValidResource(vResource<ALuint> *res) const
{
    if(!res)
        return false;

    if(res->GetParent() != this)
        return false;

    return res->IsValid();
}

bool cSoundResourceManagerBase::IsValidResource(const std::string &id) const
{    
    // Most likely will not be used, given that sound sources and buffers do not need to be distinguished
    int idx = FindResource(id);

    if(idx < 0)
        return false;
    
    return m_Resources[idx]->IsValid();
}

void cSoundResourceManagerBase::NotifyResourceParentChanged(vResource<ALuint> *res)
{
    if(!res)
        return;

    RemoveResource(res->GetStorageIndex());
}

void cSoundResourceManagerBase::Release()
{
    // Remove all resources
    // Note: if there are any valid resource proxies, they'll crash when their associated resource is dereferenced using cSoundBuffer::Get()  
    while(GetResourcesCount())
        RemoveResource(GetResourcesCount() - 1);
}

int cSoundResourceManagerBase::FindResource(const std::string &id) const
{
    // TODO: check whether this line shows up as hotspot during profiling,
    // optimize if so
    for(unsigned int i = 0; i < m_Resources.size(); ++i)
    {
        if(m_Resources[i]->GetID() == id)
            return i;
    }

    return -1;
}

int cSoundResourceManagerBase::FindResource(vResource<ALuint> *res) const
{
    if(!res)
        return -1;

    for(unsigned int i = 0; i < GetResourcesCount(); ++i)
    {
        if(m_Resources[i] == res)
            return i;
    }

    return -1;
}

bool cSoundResourceManagerBase::AddResource(vResource<ALuint> *res)
{
    // Check whether resource we're trying to claim is really sound buffer
    if(!IsResourceTypeValid(res))
        return false;

    // If we've already claimed this resource we cannot add it again
    if(FindResource(res->GetID()) >= 0)
    {
        return false;
    }

    // Claim resource
    res->SetParent(this);
    m_Resources.push_back(res);
    // Set storage index for fast access
    res->SetStorageIndex(m_Resources.size() - 1);

    return true;
}

void cSoundResourceManagerBase::RemoveResource(unsigned int storage_index)
{
    if(storage_index >= GetResourcesCount())
        return;

    vResource<ALuint> *res = m_Resources[storage_index];

    // Simple non-stable O(1) removal of array element 
    if(storage_index == GetResourcesCount() - 1)
    {
        m_Resources.pop_back();
    }
    else
    {
        m_Resources[storage_index] = m_Resources.back();
        // Update storage index
        m_Resources[storage_index]->SetStorageIndex(storage_index);
    }

    // Remove all traces of resource ever existing
    UnloadResource(res);

    if(res)
        delete res;
}
