#include "SoundBufferManager.h"
#include "IDGenerator.h"
#include "SoundBuffer.h"
#include "OpenAL.h"

cSoundBufferManager::cSoundBufferManager()
: m_IdGenerator("SoundBuffer") // All resources will be named "SoundBuffer@i"
{ 
};

cSoundBufferManager::~cSoundBufferManager()
{
    // Release all remainig resources
    Release(); 
};

cSoundBuffer* cSoundBufferManager::Get()
{
    // Create new resource
    vSoundBufferResource* res = CreateResource();
    // Claim it 
    if(!AddResource(res))
    {
        delete res;
        res = NULL;
    }
    // Return proxy
    return new cSoundBuffer(res);
};

void cSoundBufferManager::Update(float dt)
{
    // No updates needed here
};

unsigned int cSoundBufferManager::GetResourcesCount() const
{
    return m_Resources.size();
};

bool cSoundBufferManager::ReloadAll()
{
    bool succed = true;
    for(unsigned int i = 0; i < m_Resources.size(); ++i)
    {
        succed &= ReloadResource(i);
    }

    return succed;
};

bool cSoundBufferManager::ReloadResource(const std::string &id)
{
    return ReloadResource(FindResource(id));
};

void cSoundBufferManager::ReleaseResource(cSoundBuffer *res)
{
    if(!res)
        return;

    vSoundBufferResource *base = res->GetRawResource();
    // Check if its claimed
    if(base && base->GetParent() == this)
        RemoveResource(base->GetStorageIndex());
}

// TODO: rename it to IsValid
// resources loading will be performed by loaders to simplify code
bool cSoundBufferManager::IsValidResource(vResource<ALuint> *res) const
{
    if(!res)
        return false;

    if(res->GetParent() != this)
        return false;

    return res->IsValid();
}

bool cSoundBufferManager::IsValidResource(const std::string &id) const
{
    // Most likely will not be used, given that sound buffers do not need to be distinguished
    int idx = FindResource(id);

    if(idx < 0)
        return false;
    
    return m_Resources[idx]->IsValid();
};

bool cSoundBufferManager::IsValidResource(cSoundBuffer *res) const
{
    if(!res || !res->GetRawResource())
        return false;

    // Resource could be deleted at this point, therefore we cannot simply call IsValid()
    vSoundBufferResource *raw = res->GetRawResource();
    
    int index = FindResource(raw);
    
    if(index < 0)
        return false;

    return m_Resources[index]->IsValid();
};

// Most likely will never be called, there is no need for multiple sound buffer managers
void cSoundBufferManager::NotifyResourceParentChanged(vResource<ALuint> *res)
{
    if(!res)
        return;

    RemoveResource(res->GetStorageIndex());
};

void cSoundBufferManager::Release()
{
    // Remove all resources
    // Note: if there are any valid cSoundBuffer's, they'll crash when their associated resource is dereferenced using cSoundBuffer::Get()  
    while(GetResourcesCount())
        RemoveResource(GetResourcesCount() - 1);
};

int cSoundBufferManager::FindResource(const std::string &id) const
{
    // TODO: check whether this line shows up as hotspot during profiling,
    // optimize if so
    for(unsigned int i = 0; i < m_Resources.size(); ++i)
    {
        if(m_Resources[i]->GetID() == id)
            return i;
    }

    return -1;
};

int cSoundBufferManager::FindResource(vSoundBufferResource *res) const
{
    if(!res)
        return -1;

    for(unsigned int i = 0; i < GetResourcesCount(); ++i)
    {
        if(m_Resources[i] == res)
            return i;
    }

    return -1;
};

bool cSoundBufferManager::AddResource(vSoundBufferResource *res)
{
    // Check whether resource we're trying to claim is really sound buffer
    if(!res || res->GetType() != EResourceType::OalSoundBuffer)
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

void cSoundBufferManager::RemoveResource(unsigned int storage_index)
{
    if(storage_index >= GetResourcesCount())
        return;

    vSoundBufferResource *res = m_Resources[storage_index];

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

bool cSoundBufferManager::ReloadResource(unsigned int storage_index)
{
    if(storage_index >= GetResourcesCount())
        return false;

    vSoundBufferResource *rs = m_Resources[storage_index];
    if(!rs)
        return false;

    ALuint buffer = 0;
    // Reloading starts by releasing associated buffer
    UnloadResource(rs);
    // Recreate buffer and update resource state
    buffer = OpenAL::Get().CreateBuffer();
    if(alIsBuffer(buffer))
    {
        rs->SetState(EResourceState::Ok);
        rs->SetResource(buffer);
        return true;
    }

    return false;
}

vSoundBufferResource* cSoundBufferManager::CreateResource()
{    
    // Create new resource
    vSoundBufferResource *sb = new vSoundBufferResource(OpenAL::Get().CreateBuffer());

    // Claim ownership
    sb->SetParent(this);
    sb->SetID(m_IdGenerator.Next());
    sb->SetState(EResourceState::NotLoaded);

    // TODO: make vResource validate itself
    if(alIsBuffer(sb->Get()) == AL_TRUE)
        sb->SetState(EResourceState::Ok);

    return sb;
}

void cSoundBufferManager::UnloadResource(vSoundBufferResource *res)
{
    // Check ownership etc.
    if(!res || res->GetParent() != this)
        return;

    ALuint buffer = res->Get();

    // TODO: vResource should check whether it is valid
    if(res->GetState() == EResourceState::Ok && alIsBuffer(buffer) == AL_TRUE)
    {
        res->SetState(EResourceState::Deleted);
        OpenAL::Get().DeleteBuffer(buffer);
        res->SetResource(0);
    }
}