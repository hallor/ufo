#include "SoundSourceManager.h"
#include "SoundSource.h"
#include "OpenAL.h"

cSoundSourceManager::cSoundSourceManager()
: m_IdGenerator("SoundSource")
{
};

cSoundSource* cSoundSourceManager::Get()
{
    vSoundSourceResource *res = CreateResource();

    if(!AddResource(res))
    {
        delete res;
        res = NULL;
    }

    return new cSoundSource(res);
};


void cSoundSourceManager::ReleaseResource(cSoundSource *res)
{
    if(!res)
        return;

    vSoundSourceResource *base = res->GetRawResource();
    // Check if its claimed
    if(base && base->GetParent() == this)
        RemoveResource(base->GetStorageIndex());
}

bool cSoundSourceManager::IsValidResource(cSoundSource *res) const
{
    if(!res || !res->GetRawResource())
				return false;

    // Resource could be deleted at this point, therefore we cannot simply call IsValid()
    vSoundSourceResource *raw = res->GetRawResource();
    
    int index = FindResource(raw);
    
    if(index < 0)
        return false;

    return m_Resources[index]->IsValid();
};

bool cSoundSourceManager::ReloadResource(unsigned int storage_index)
{
    if(storage_index >= GetResourcesCount())
        return false;

    vSoundSourceResource *rs = (vSoundSourceResource*)m_Resources[storage_index];
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

vSoundSourceResource* cSoundSourceManager::CreateResource()
{    
    // Create new resource
    vSoundSourceResource *ss = new vSoundSourceResource(OpenAL::Get().CreateSource());

    // Claim ownership
    ss->SetParent(this);
    ss->SetID(m_IdGenerator.Next());
    ss->SetState(EResourceState::NotLoaded);

    // TODO: make vResource validate itself
    if(alIsSource(ss->Get()) == AL_TRUE)
        ss->SetState(EResourceState::Ok);

    return ss;
}

bool cSoundSourceManager::IsResourceTypeValid(vResource<ALuint> *res)
{
    if(!res || res->GetType() != EResourceType::OalSoundSource)
        return false;

    return true;
}

void cSoundSourceManager::UnloadResource(vResource<ALuint> *res)
{
    // Check ownership etc.
    if(!res || res->GetParent() != this)
        return;

    ALuint source = res->Get();

    // TODO: vResource should check whether it is valid
    if(res->GetState() == EResourceState::Ok && alIsSource(source) == AL_TRUE)
    {
        res->SetState(EResourceState::Deleted);
        OpenAL::Get().DeleteSource(source);
        res->SetResource(0);
    }
}

