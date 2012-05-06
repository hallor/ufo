#include "TextureManager.h"
#include "Texture.h"

cTextureManager::cTextureManager()
{
}

cTextureManager::~cTextureManager()
{
    ReleaseAll();
}

cTexture *cTextureManager::Get(const std::string &str)
{
    vTextureResource *res= NULL;

    int index = FindResource(str);
    if(index < 0)
    {
        res = CreateResource(str, LoadTexture(str));
        if(res != NULL)
        {
            m_Resources.push_back(res);
            m_Resources.back()->SetStorageIndex(m_Resources.size() - 1);
        }
    }
    else
        res = m_Resources[index];

    return new cTexture(res);
}

void cTextureManager::Update(float dt)
{
}

unsigned int cTextureManager::GetResourcesCount() const
{
    return m_Resources.size();
}

bool cTextureManager::ReloadAll()
{
    return false;
}

bool cTextureManager::ReloadResource(const std::string &id)
{
    return false;
}

bool cTextureManager::IsValidResource(vResource<SDL_Surface*> *res) const
{
    int index = FindResource(res);
    if(index < 0)
        return false;

    return m_Resources[index]->IsValid();
}

bool cTextureManager::IsValidResource(const std::string &res) const
{
    int index = FindResource(res);
    if(index < 0)
        return false;

    return m_Resources[index]->IsValid();
}

bool cTextureManager::IsValidResource(cTexture *tex) const
{
    if(!tex || !tex->GetRawResource())
        return false;

    if(tex->GetRawResource()->GetParent() != this)
        return false;

    int index = FindResource(tex->GetRawResource());
    if(index < 0)
        return false;

    return m_Resources[index]->IsValid();
}

void cTextureManager::NotifyResourceParentChanged(vResource<SDL_Surface*> *res)
{
    int index = FindResource(res);
    if(index >= 0)
        RemoveResource(index);
}

SDL_Surface *cTextureManager::LoadTexture(const std::string &file)
{
     if(!file.length())
        return NULL;

    SDL_Surface *tmp = SDL_LoadBMP(file.c_str());
    if(!tmp)
        return NULL;

    SDL_Surface *ret = SDL_DisplayFormat(tmp);
    
    SDL_FreeSurface(tmp);

    SDL_SetColorKey(ret, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(ret->format, 0, 0, 0));

    return ret;
}

vTextureResource *cTextureManager::CreateResource(const std::string &file, SDL_Surface *surf)
{
    vTextureResource *res = new vTextureResource(surf);
    res->SetParent(this);
    res->SetState(surf ? EResourceState::Ok : EResourceState::NotLoaded);
    res->SetID(file);
    
    return res;
}

int cTextureManager::FindResource(const std::string &id) const
{
    for(unsigned int i = 0; i < GetResourcesCount(); ++i)
    {
        if(m_Resources[i]->GetID() == id)
            return i;
    }

    return -1;
}

int cTextureManager::FindResource(vResource<SDL_Surface*> *res) const
{
    if(!res || res->GetParent() != this)
        return -1;

    if(res->GetType() != EResourceType::SDLSurface)
        return -1;

    return FindResource(res->GetID());
}

void cTextureManager::RemoveResource(unsigned int storage_index)
{
    if(storage_index >= GetResourcesCount())
        return;
    
    m_Resources[storage_index] = m_Resources.back();
    m_Resources[storage_index]->SetStorageIndex(storage_index);
    m_Resources.pop_back();
}

void cTextureManager::DeleteResource(unsigned int storage_index)
{
    if(storage_index >= GetResourcesCount())
        return;

    vTextureResource *res = m_Resources[storage_index];
    RemoveResource(storage_index);
    ReleaseResource(res);
}

void cTextureManager::ReleaseResource(vTextureResource *res)
{
    if(!res)
        return;

    if(res->GetParent() != this)
        return;
    
    SDL_Surface *raw = res->Get();
    if(raw)
        SDL_FreeSurface(raw);

    delete res;
}

void cTextureManager::ReleaseAll()
{
    for(unsigned int i = 0; i < m_Resources.size(); ++i)
        ReleaseResource(m_Resources[i]);

    m_Resources.clear();
}

