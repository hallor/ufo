#include "SoundBufferManager.h"
#include "IDGenerator.h"
#include "SoundBuffer.h"
#include "OpenAL.h"

cSoundBufferManager::cSoundBufferManager()
: m_IdGenerator("SoundBuffer") // All resources will be named "SoundBuffer@i"
{ 
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

void cSoundBufferManager::ReleaseResource(cSoundBuffer *res)
{
    if(!res)
        return;

    vSoundBufferResource *base = res->GetRawResource();
    // Check if its claimed
    if(base && base->GetParent() == this)
        RemoveResource(base->GetStorageIndex());
}

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

bool cSoundBufferManager::ReloadResource(unsigned int storage_index)
{
    if(storage_index >= GetResourcesCount())
        return false;

    vSoundBufferResource *rs = (vSoundBufferResource*)m_Resources[storage_index];
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

bool cSoundBufferManager::IsResourceTypeValid(vResource<ALuint> *res)
{
    if(!res || res->GetType() != EResourceType::OalSoundBuffer)
        return false;

    return true;
}

void cSoundBufferManager::UnloadResource(vResource<ALuint> *res)
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
