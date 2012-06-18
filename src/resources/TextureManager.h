#pragma once
#include "compat_sdl.h"
#include <vector>
#include "ManagerBase.h"
#include "ResourceBase.h"
#include "Texture.h"


class vTextureResource;

class cTextureManager : public iManagerBase<SDL_Surface *>
{
public:
    cTextureManager();
    virtual ~cTextureManager();

    virtual cTexture Get(const std::string& id);

    virtual void Update(float dt);
    virtual unsigned int GetResourcesCount() const;

    virtual bool ReloadAll();
    virtual bool ReloadResource(const std::string &id);
    
    virtual bool IsValidResource(vResource<SDL_Surface*> *res) const;
    virtual bool IsValidResource(const std::string &id) const;
    virtual bool IsValidResource(const cTexture &tex) const;

    virtual void NotifyResourceParentChanged(vResource<SDL_Surface*> *res);

protected:
    virtual SDL_Surface *LoadTexture(const std::string &file);
    virtual vTextureResource* CreateResource(const std::string &file, SDL_Surface *surf);

    virtual int FindResource(const std::string &id) const;
    virtual int FindResource(vResource<SDL_Surface*> *res) const;

    virtual void RemoveResource(unsigned int storage_index);    
    virtual void DeleteResource(unsigned int storage_index);

    virtual void ReleaseResource(vTextureResource *res);
    virtual void ReleaseAll();
        
    std::vector<vTextureResource*> m_Resources;
};
